#include "../../../inc/ciphers/block/MARS.hpp"

namespace crypto {
    bytes_t MARS::encrypt(const bytes_t& plaintext)
    {
        return encrypt_block(plaintext);
    }

    bytes_t MARS::decrypt(const bytes_t& ciphertext)
    {
        return decrypt_block(ciphertext);
    }

    void MARS::validate(const bytes_t& block) const
    {
        if (!initialized_) throw std::logic_error("MARS: Key is not initialized.");
        if (block.size() != 16) throw std::invalid_argument("MARS: Block size must be 16 bytes.");
    }

    void MARS::setupKeys(const bytes_t& key)
    {
        if (key.size() < 16 || key.size() > 56 || key.size() % 4 != 0) throw std::invalid_argument("MARS: Key size must be 16-56 bytes, multiple of 4.");

        int n = static_cast<int>(key.size() / 4);
        uint32_t T[15] = {0};

        for (int i = 0; i < n; ++i)
        {
            T[i] = static_cast<uint32_t>(key[i*4]) |
                   (static_cast<uint32_t>(key[i*4+1]) << 8) |
                   (static_cast<uint32_t>(key[i*4+2]) << 16) |
                   (static_cast<uint32_t>(key[i*4+3]) << 24);
        }
        T[n] = n;

        const uint32_t B[4] = {0xa4a8d57b, 0x5b5d193b, 0xc8a8309b, 0x73f9a978};

        for (int j = 0; j < 4; ++j)
        {
            for (int i = 0; i < 15; ++i)
            {
                int i7 = (i + 8) % 15;
                int i2 = (i + 13) % 15;
                T[i] ^= rol32(T[i7] ^ T[i2], 3) ^ (4 * i + j);
            }

            for (int s = 0; s < 4; ++s)
            {
                for (int i = 0; i < 15; ++i)
                {
                    int prev = (i + 14) % 15;
                    T[i] = rol32(T[i] + S[T[prev] & 0x1FF], 9);
                }
            }

            for (int i = 0; i < 10; ++i)
            {
                EK_[10 * j + i] = T[(4 * i) % 15];
            }
        }

        for (int ii = 5; ii <= 35; ii += 2)
        {
            uint32_t Ki = EK_[ii];
            int j = Ki & 0x3;
            uint32_t w = Ki | 0x3;

            uint32_t M = 0;
            for (int start = 0; start <= 22; ++start)
            {
                uint32_t bit = (w >> start) & 1;
                bool run = true;
                for (int k = 0; k < 10; ++k)
                {
                    if (((w >> (start + k)) & 1) != bit)
                    {
                        run = false; break;
                    }
                }
                if (run)
                {
                    for (int pos = start; pos < start + 10; ++pos)
                    {
                        if (pos >= 2 && pos <= 30) M |= (1u << pos);
                    }
                }
            }
            M &= ~0x80000003u;

            for (int k = 2; k <= 30; ++k)
            {
                uint32_t bk = (w >> k) & 1;
                if (bk != ((w >> (k-1)) & 1) || bk != ((w >> (k+1)) & 1))
                {
                    M &= ~(1u << k);
                }
            }

            uint32_t r = EK_[ii - 1] & 0x1F;
            uint32_t p = rol32(B[j], r);
            EK_[ii] = w ^ (p & M);
        }

        initialized_ = true;
    }

    bytes_t MARS::encrypt_block(const bytes_t& block)
    {
        validate(block);
        uint32_t D[4];
        for (int i = 0; i < 4; ++i)
        {
            D[i] = static_cast<uint32_t>(block[i*4]) |
                   (static_cast<uint32_t>(block[i*4+1]) << 8) |
                   (static_cast<uint32_t>(block[i*4+2]) << 16) |
                   (static_cast<uint32_t>(block[i*4+3]) << 24);
            D[i] += EK_[i];
        }

        for (int i = 0; i < 8; ++i)
        {
            D[1] = (D[1] ^ S[D[0] & 0xFF]) + S[256 + ((D[0] >> 8) & 0xFF)];
            D[2] += S[(D[0] >> 16) & 0xFF];
            D[3] ^= S[256 + ((D[0] >> 24) & 0xFF)];

            D[0] = ror32(D[0], 24);
            if (i == 0 || i == 4) D[0] += D[3];
            if (i == 1 || i == 5) D[0] += D[1];

            uint32_t t = D[0]; D[0] = D[1]; D[1] = D[2]; D[2] = D[3]; D[3] = t;
        }

        for (int i = 0; i < 16; ++i)
        {
            uint32_t R = rol32(D[0], 13) * EK_[2*i + 5];
            uint32_t M = D[0] + EK_[2*i + 4];
            uint32_t L = S[M & 0x1FF];

            R = rol32(R, 5);
            uint32_t r1 = R & 0x1F;
            M = rol32(M, r1);
            L ^= R;

            R = rol32(R, 5);
            L ^= R;
            uint32_t r2 = R & 0x1F;
            L = rol32(L, r2);

            if (i < 8)
            {
                D[1] += L;
                D[3] ^= R;
            }
            else
            {
                D[3] += L;
                D[1] ^= R;
            }
            D[2] += M;

            uint32_t temp = rol32(D[0], 13);

            D[0] = D[1];
            D[1] = D[2];
            D[2] = D[3];
            D[3] = temp;
        }

        for (int i = 0; i < 8; ++i)
        {
            if (i == 2 || i == 6) D[0] -= D[3];
            if (i == 3 || i == 7) D[0] -= D[1];

            D[1] ^= S[256 + (D[0] & 0xFF)];
            D[2] -= S[D[0] >> 24];
            D[3] = (D[3] - S[256 + ((D[0] >> 16) & 0xFF)]) ^ S[(D[0] >> 8) & 0xFF];

            D[0] = rol32(D[0], 24);

            uint32_t t = D[0];
            D[0] = D[1];
            D[1] = D[2];
            D[2] = D[3];
            D[3] = t;
        }

        bytes_t res(16);
        for (int i = 0; i < 4; ++i)
        {
            D[i] -= EK_[36 + i];
            res[i*4] = static_cast<uint8_t>(D[i] & 0xFF);
            res[i*4+1] = static_cast<uint8_t>((D[i] >> 8) & 0xFF);
            res[i*4+2] = static_cast<uint8_t>((D[i] >> 16) & 0xFF);
            res[i*4+3] = static_cast<uint8_t>((D[i] >> 24) & 0xFF);
        }
        return res;
    }

    bytes_t MARS::decrypt_block(const bytes_t& block) const
    {
        validate(block);
        uint32_t D[4];
        for (int i = 0; i < 4; ++i)
        {
            D[i] = static_cast<uint32_t>(block[i*4]) |
                   (static_cast<uint32_t>(block[i*4+1]) << 8) |
                   (static_cast<uint32_t>(block[i*4+2]) << 16) |
                   (static_cast<uint32_t>(block[i*4+3]) << 24);
            D[i] += EK_[36 + i];
        }

        for (int i = 7; i >= 0; --i)
        {
            uint32_t t = D[3]; D[3] = D[2]; D[2] = D[1]; D[1] = D[0]; D[0] = t;
            D[0] = ror32(D[0], 24);

            D[3] = (D[3] ^ S[(D[0] >> 8) & 0xFF]) + S[256 + ((D[0] >> 16) & 0xFF)];
            D[2] += S[D[0] >> 24];
            D[1] ^= S[256 + (D[0] & 0xFF)];

            if (i == 2 || i == 6) D[0] += D[3];
            if (i == 3 || i == 7) D[0] += D[1];
        }

        for (int i = 15; i >= 0; --i)
        {
            uint32_t temp = ror32(D[3], 13);

            D[3] = D[2];
            D[2] = D[1];
            D[1] = D[0];
            D[0] = temp;

            uint32_t R = rol32(D[0], 13) * EK_[2*i + 5];
            uint32_t M = D[0] + EK_[2*i + 4];
            uint32_t L = S[M & 0x1FF];

            R = rol32(R, 5);
            uint32_t r1 = R & 0x1F;
            M = rol32(M, r1);
            L ^= R;

            R = rol32(R, 5);
            L ^= R;
            uint32_t r2 = R & 0x1F;
            L = rol32(L, r2);

            D[2] -= M;
            if (i < 8)
            {
                D[1] -= L;
                D[3] ^= R;
            }
            else
            {
                D[3] -= L;
                D[1] ^= R;
            }
        }

        for (int i = 7; i >= 0; --i)
        {
            uint32_t t = D[3]; D[3] = D[2]; D[2] = D[1]; D[1] = D[0]; D[0] = t;

            if (i == 1 || i == 5) D[0] -= D[1];
            if (i == 0 || i == 4) D[0] -= D[3];

            D[0] = rol32(D[0], 24);
            D[3] ^= S[256 + ((D[0] >> 24) & 0xFF)];
            D[2] -= S[(D[0] >> 16) & 0xFF];
            D[1] = (D[1] - S[256 + ((D[0] >> 8) & 0xFF)]) ^ S[D[0] & 0xFF];
        }

        bytes_t res(16);
        for (int i = 0; i < 4; ++i)
        {
            D[i] -= EK_[i];
            res[i*4]   = static_cast<uint8_t>(D[i] & 0xFF);
            res[i*4+1] = static_cast<uint8_t>((D[i] >> 8) & 0xFF);
            res[i*4+2] = static_cast<uint8_t>((D[i] >> 16) & 0xFF);
            res[i*4+3] = static_cast<uint8_t>((D[i] >> 24) & 0xFF);
        }
        return res;
    }

    size_t MARS::getBlockSize() const { return 16; }



} // namespace crypto