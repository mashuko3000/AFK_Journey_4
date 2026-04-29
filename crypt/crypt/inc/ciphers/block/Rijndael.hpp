//
// Created by maria on 07.04.2026.
//

#ifndef CRYPT_RIJNDAEL_HPP
#define CRYPT_RIJNDAEL_HPP

#include"common/RijndaelTypes.hpp"
#include"../../layers/RijndaelSBox.hpp"
#include"core/IBlockCipher.hpp"
#include "../../math/GaloisField.hpp"


namespace crypto
{
    template <RijndaelBlockSize Block = RijndaelBlockSize::Bits128,
            RijndaelKeySize Key = RijndaelKeySize::Bits128>

    class Rijndael final : public IBlockCipher
    {
        GaloisField<8> _gf;

        std::array<byte_t, 256> _sbox_table;
        std::array<byte_t, 256> _inv_sbox_table;

        bytes_t _round_keys;

    public:
        static constexpr size_t kBlockBits = static_cast<size_t>(Block);
        static constexpr size_t kKeyBits = static_cast<size_t>(Key);
        static constexpr size_t kBlockBytes = kBlockBits >> 3;
        static constexpr size_t kKeyBytes = kKeyBits >> 3;

        static constexpr size_t NumColmnsInBlock = kBlockBits >> 5;
        static constexpr size_t NumColmnsInKey = kKeyBits >> 5;
        static constexpr size_t NumRounds = std::max(NumColmnsInBlock, NumColmnsInKey) + 6;

        static_assert(NumColmnsInBlock == 4 || NumColmnsInBlock == 6 || NumColmnsInBlock == 8, "Unsupported block size");
        static_assert(NumColmnsInKey == 4 || NumColmnsInKey == 6 || NumColmnsInKey == 8, "Unsupported Key size");

    private:
        using State = std::array<uint8_t, kBlockBytes>;
        [[nodiscard]] static GaloisField<8> defaultField()
        {
            return GaloisField<8>(GFPolynomial<8>{0x11bULL});
        }

    public:
        explicit Rijndael(GaloisField<8> field = defaultField()) : _gf(std::move(field))
        {
            _sbox_table = RijndaelSBox::buildSBox(_gf);
            _inv_sbox_table = RijndaelSBox::buildInvSBox(_sbox_table);
        }
        ~Rijndael() override = default;

        Rijndael(const Rijndael&) = delete;
        Rijndael& operator=(const Rijndael&) = delete;
        Rijndael(Rijndael&&) noexcept = default;
        Rijndael& operator=(Rijndael&&) noexcept = default;

    public:
        [[nodiscard]] virtual std::string name() const override
        {
            return "<< Rijndael " + std::to_string(kBlockBits) + " - " + std::to_string(kKeyBits) + " >>";
        }

        virtual void setupKeys(const bytes_t& key) override
        {
            if(key.size() != kKeyBytes)
            {
                throw std::invalid_argument("Invalid key size");
            }
            keyExpansion(key);
        }

        [[nodiscard]] virtual size_t getBlockSize() const override
        {
            return kBlockBytes;
        }

        [[nodiscard]] virtual bytes_t encrypt_block(const bytes_t& block) override
        {
            if(block.size() != kBlockBytes)
            {
                throw std::invalid_argument("Invalid block size");
            }

            State state;
            std::copy(block.begin(), block.end(), state.begin());

            addRoundKey(state, 0);

            for(size_t i = 1; i < NumRounds; ++i)
            {
                subBytes(state, false);
                shiftRows(state, false);
                mixColumns(state, false);
                addRoundKey(state, i);
            }

            subBytes(state, false);
            shiftRows(state, false);
            addRoundKey(state, NumRounds);

            return bytes_t(state.begin(), state.end());
        }

        [[nodiscard]] virtual bytes_t decrypt_block(const bytes_t& block) const override
        {
            if(block.size() != kBlockBytes)
            {
                throw std::invalid_argument("Invalid block size");
            }

            State state;
            std::copy(block.begin(), block.end(), state.begin());

            addRoundKey(state, NumRounds);

            for(size_t i = NumRounds - 1; i > 0; --i)
            {

                shiftRows(state, true);
                subBytes(state, true);

                addRoundKey(state, i);
                mixColumns(state, true);

            }

            shiftRows(state, true);
            subBytes(state, true);
            addRoundKey(state, 0);

            return bytes_t(state.begin(), state.end());
        }

        [[nodiscard]] virtual bytes_t encrypt(const bytes_t& plaintext)  override
        {
            throw std::logic_error("Use CipherContext with a specific Mode (ECB, CBC) to encrypt arbitrary data");
        }

        [[nodiscard]] virtual bytes_t decrypt(const bytes_t& ciphertext) override
        {
            throw std::logic_error("Use CipherContext with a specific Mode to decrypt arbitrary data");
        }


    private:
        void addRoundKey(State& state, const size_t& round) const
        {
            size_t offset = round * kBlockBytes;
            for(size_t i = 0; i < kBlockBytes; ++i)
            {
                state[i] ^= _round_keys[offset + i];
            }
        }
        void subBytes(State& state, bool inverse) const
        {
            for (uint8_t& byte : state)
            {
                byte = inverse ? _inv_sbox_table[byte] : _sbox_table[byte];
            }
        }
        void shiftRows(State& state, bool inverse) const
        {
            State temp = state;

            const size_t c1 = 1;
            const size_t c2 = (NumColmnsInBlock == 8) ? 3 : 2;
            const size_t c3 = (NumColmnsInBlock == 8) ? 4 : 3;

            for (size_t r = 1; r < 4; ++r)
            {
                size_t shift = (r == 1)
                        ? c1
                        : (r == 2)
                            ? c2
                            : c3;

                for (size_t c = 0; c < NumColmnsInBlock; ++c)
                {
                    size_t new_c = inverse
                            ? (c + NumColmnsInBlock - shift) % NumColmnsInBlock
                            : (c + shift) % NumColmnsInBlock;

                    state[r + c * 4] = temp[r + new_c * 4];
                }
            }
        }
        void mixColumns(State& state, bool inverse) const
        {
            State temp = state;
            for (size_t c = 0; c < NumColmnsInBlock; ++c)
            {
                size_t offset = c * 4;
                GFPolynomial<8> v0(temp[offset + 0]);
                GFPolynomial<8> v1(temp[offset + 1]);
                GFPolynomial<8> v2(temp[offset + 2]);
                GFPolynomial<8> v3(temp[offset + 3]);

                if (!inverse)
                {
                    state[offset + 0] = static_cast<uint8_t>((_gf.mul(v0, GFPolynomial<8>(0x02)) + _gf.mul(v1, GFPolynomial<8>(0x03)) + v2 + v3).value());
                    state[offset + 1] = static_cast<uint8_t>((v0 + _gf.mul(v1, GFPolynomial<8>(0x02)) + _gf.mul(v2, GFPolynomial<8>(0x03)) + v3).value());
                    state[offset + 2] = static_cast<uint8_t>((v0 + v1 + _gf.mul(v2, GFPolynomial<8>(0x02)) + _gf.mul(v3, GFPolynomial<8>(0x03))).value());
                    state[offset + 3] = static_cast<uint8_t>((_gf.mul(v0, GFPolynomial<8>(0x03)) + v1 + v2 + _gf.mul(v3, GFPolynomial<8>(0x02))).value());
                }
                else
                {
                    GFPolynomial<8> p0e(0x0e), p0b(0x0b), p0d(0x0d), p09(0x09);
                    state[offset + 0] = static_cast<uint8_t>((_gf.mul(v0, p0e) + _gf.mul(v1, p0b) + _gf.mul(v2, p0d) + _gf.mul(v3, p09)).value());
                    state[offset + 1] = static_cast<uint8_t>((_gf.mul(v0, p09) + _gf.mul(v1, p0e) + _gf.mul(v2, p0b) + _gf.mul(v3, p0d)).value());
                    state[offset + 2] = static_cast<uint8_t>((_gf.mul(v0, p0d) + _gf.mul(v1, p09) + _gf.mul(v2, p0e) + _gf.mul(v3, p0b)).value());
                    state[offset + 3] = static_cast<uint8_t>((_gf.mul(v0, p0b) + _gf.mul(v1, p0d) + _gf.mul(v2, p09) + _gf.mul(v3, p0e)).value());
                }
            }
        }
        void keyExpansion(const bytes_t& master_key)
        {
            _round_keys.resize(kBlockBytes * (NumRounds + 1));
            std::vector<uint32_t> words(NumColmnsInBlock * (NumRounds + 1));

            for (size_t i = 0; i < NumColmnsInKey; ++i)
            {
                words[i] = (master_key[4 * i] << 24) | (master_key[4 * i + 1] << 16) |
                           (master_key[4 * i + 2] << 8) | master_key[4 * i + 3];
            }
            
            uint8_t rcon_val = 0x01;
            for (size_t i = NumColmnsInKey ; i < words.size(); ++i)
            {
                uint32_t temp = words[i - 1];
                if (i % NumColmnsInKey == 0)
                {
                    temp = (temp << 8) | (temp >> 24);
                    temp = (_sbox_table[(temp >> 24) & 0xFF] << 24) |
                           (_sbox_table[(temp >> 16) & 0xFF] << 16) |
                           (_sbox_table[(temp >> 8) & 0xFF] << 8) |
                           (_sbox_table[temp & 0xFF]);
                    temp ^= (static_cast<uint32_t>(rcon_val) << 24);
                    rcon_val = static_cast<uint8_t>(_gf.mul(GFPolynomial<8>(rcon_val), GFPolynomial<8>(0x02)).value());
                }
                else if (NumColmnsInKey > 6 && i % NumColmnsInKey == 4)
                {
                    temp = (_sbox_table[(temp >> 24) & 0xFF] << 24) |
                           (_sbox_table[(temp >> 16) & 0xFF] << 16) |
                           (_sbox_table[(temp >> 8) & 0xFF] << 8) |
                           (_sbox_table[temp & 0xFF]);
                }
                words[i] = words[i - NumColmnsInKey] ^ temp;
            }

            for (size_t i = 0; i < words.size(); ++i)
            {
                _round_keys[i * 4 + 0] = (words[i] >> 24) & 0xFF;
                _round_keys[i * 4 + 1] = (words[i] >> 16) & 0xFF;
                _round_keys[i * 4 + 2] = (words[i] >> 8)  & 0xFF;
                _round_keys[i * 4 + 3] = words[i] & 0xFF;
            }
        }
    };
}

#endif //CRYPT_RIJNDAEL_HPP
