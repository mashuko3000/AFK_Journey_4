//
// Created by maria on 13.04.2026.
//
#include"../../../inc/ciphers/block/TwoFish.hpp"
#include <algorithm>

#include<stdexcept>

namespace crypto
{
    TwoFish::TwoFish() : gfMDS(GFPolynomial<8>(0x169)), gfRS(GFPolynomial<8>(0x14D)){}

    void TwoFish::setupKeys(const bytes_t& key)
    {
        size_t keySize = key.size();
        if(keySize != 16 && keySize != 24 && keySize != 32)
        {
            throw std::invalid_argument("Twofish: Key size must be 128, 192, or 256 bits");
        }

        k_ = static_cast<int>(keySize / 8);

        std::vector<uint32_t> M(k_ * 2);

        for (int i = 0; i < k_ * 2; ++i)
        {
            M[i] = key[i * 4] | (key[i * 4 + 1] << 8) | (key[i * 4 + 2] << 16) | (key[i * 4 + 3] << 24);
        }

        std::vector<uint32_t> Me(k_), Mo(k_);
        sBoxKeys_.fill(0);

        for(int i = 0; i<k_; ++i)
        {
            Me[i] = M[2 * i];
            Mo[i] = M[2 * i + 1];
            sBoxKeys_[k_ - 1 - i] = multiplyRS(&key[i * 8]);
        }

        for (int i = 0; i < 20; ++i)
        {
            uint32_t A = HFunc(2 * i * 0x01010101, Me.data());
            uint32_t B = rol32(HFunc((2 * i + 1) * 0x01010101, Mo.data()), 8);
            subKeys_[2 * i] = A + B;
            subKeys_[2 * i + 1] = rol32(A + 2 * B, 9);
        }
        initialized_ = true;
    }

    uint32_t TwoFish::HFunc(const uint32_t X, const uint32_t* L) const noexcept
    {
        uint8_t y[4] =
                {
                static_cast<uint8_t>(X & 0xFF),
                static_cast<uint8_t>((X >> 8)  & 0xFF),
                static_cast<uint8_t>((X >> 16) & 0xFF),
                static_cast<uint8_t>((X >> 24) & 0xFF)
        };

        if (k_ >= 4)
        {
            y[0] = Q1[y[0]] ^ ((L[3] >> 0)  & 0xFF);
            y[1] = Q0[y[1]] ^ ((L[3] >> 8)  & 0xFF);
            y[2] = Q0[y[2]] ^ ((L[3] >> 16) & 0xFF);
            y[3] = Q1[y[3]] ^ ((L[3] >> 24) & 0xFF);
        }
        if (k_ >= 3)
        {
            y[0] = Q1[y[0]] ^ ((L[2] >> 0)  & 0xFF);
            y[1] = Q1[y[1]] ^ ((L[2] >> 8)  & 0xFF);
            y[2] = Q0[y[2]] ^ ((L[2] >> 16) & 0xFF);
            y[3] = Q0[y[3]] ^ ((L[2] >> 24) & 0xFF);
        }

        y[0] = Q1[Q0[Q0[y[0]] ^ ((L[1] >> 0)  & 0xFF)] ^ ((L[0] >> 0)  & 0xFF)];
        y[1] = Q0[Q0[Q1[y[1]] ^ ((L[1] >> 8)  & 0xFF)] ^ ((L[0] >> 8)  & 0xFF)];
        y[2] = Q1[Q1[Q0[y[2]] ^ ((L[1] >> 16) & 0xFF)] ^ ((L[0] >> 16) & 0xFF)];
        y[3] = Q0[Q1[Q1[y[3]] ^ ((L[1] >> 24) & 0xFF)] ^ ((L[0] >> 24) & 0xFF)];

        return static_cast<uint32_t>(y[0])|
               (static_cast<uint32_t>(y[1]) << 8)|
               (static_cast<uint32_t>(y[2]) << 16)|
               (static_cast<uint32_t>(y[3]) << 24);
    }

    uint32_t TwoFish::multiplyRS(const uint8_t* in) const noexcept
    {
        uint8_t out[4] = {0};
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                out[i] ^= GFMultipyRS(RS[i][j], in[j]);
            }
        }
        return static_cast<uint32_t>(out[0]) | (static_cast<uint32_t>(out[1]) << 8) |
               (static_cast<uint32_t>(out[2]) << 16) | (static_cast<uint32_t>(out[3]) << 24);
    }

    uint8_t TwoFish::GFMultipyRS(uint8_t a, uint8_t b) const noexcept
    {
        try
        {
            return static_cast<uint8_t>(gfRS.mul(GFPolynomial<8>(a), GFPolynomial<8>(b)).value());
        }
        catch (...)
        {
            return 0;
        }
    }

    void TwoFish::validateBlock(const bytes_t& block) const
    {
        if(block.size() != 16)
        {
            throw std::invalid_argument("Twofish: Block must be 16 bytes");
        }
    }

    bytes_t TwoFish::encrypt_block(const bytes_t& block) {
        validateBlock(block);
        uint32_t x[4];
        for (int i = 0; i < 4; ++i)
        {
            x[i] = (block[i * 4] | (block[i * 4 + 1] << 8) | (block[i * 4 + 2] << 16) | (block[i * 4 + 3] << 24)) ^ subKeys_[i];
        }

        for (int r = 0; r < 16; ++r)
        {
            uint32_t t0 = HFunc(x[0], sBoxKeys_.data());
            uint32_t t1 = HFunc(rol32(x[1], 8), sBoxKeys_.data());

            x[2] = ror32(x[2] ^ (t0 + t1 + subKeys_[2 * r + 8]), 1);
            x[3] = rol32(x[3], 1) ^ (t0 + 2 * t1 + subKeys_[2 * r + 9]);

            if (r < 15)
            {
                std::swap(x[0], x[2]); std::swap(x[1], x[3]);
            }
        }

        bytes_t res(16);
        for (int i = 0; i < 4; ++i)
        {
            uint32_t out = x[i] ^ subKeys_[i + 4];
            res[i*4] = out & 0xFF;
            res[i*4+1] = (out >> 8) & 0xFF;
            res[i*4+2] = (out >> 16) & 0xFF;
            res[i*4+3] = (out >> 24) & 0xFF;
        }
        return res;
    }

    bytes_t TwoFish::decrypt_block(const bytes_t& block) const
    {
        validateBlock(block);
        uint32_t x[4];
        for (int i = 0; i < 4; ++i)
        {
            x[i] = (block[i*4] | (block[i*4+1] << 8) | (block[i*4+2] << 16) | (block[i*4+3] << 24)) ^ subKeys_[i + 4];
        }

        for (int r = 15; r >= 0; --r)
        {
            uint32_t t0 = HFunc(x[0], sBoxKeys_.data());
            uint32_t t1 = HFunc(rol32(x[1], 8), sBoxKeys_.data());

            x[2] = rol32(x[2], 1) ^ (t0 + t1 + subKeys_[2 * r + 8]);
            x[3] = ror32(x[3] ^ (t0 + 2 * t1 + subKeys_[2 * r + 9]), 1);

            if (r > 0)
            {
                std::swap(x[0], x[2]); std::swap(x[1], x[3]);
            }
        }

        bytes_t res(16);

        for (int i = 0; i < 4; ++i)
        {
            uint32_t out = x[i] ^ subKeys_[i];
            res[i*4] = out & 0xFF;
            res[i*4+1] = (out >> 8) & 0xFF;
            res[i*4+2] = (out >> 16) & 0xFF;
            res[i*4+3] = (out >> 24) & 0xFF;
        }

        return res;
    }

    bytes_t TwoFish::encrypt(const bytes_t& plaintext)
    {
        return encrypt_block(plaintext);
    }

    bytes_t TwoFish::decrypt(const bytes_t& ciphertext)
    {
        return decrypt_block(ciphertext);
    }
}