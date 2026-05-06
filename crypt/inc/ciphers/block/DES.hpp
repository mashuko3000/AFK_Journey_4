//
// Created by maria on 30.04.2026.
//

#ifndef CRYPT_DES_HPP
#define CRYPT_DES_HPP

#include "des.hpp"
#include "../../core/IBlockCipher.hpp"

namespace crypto
{
    class DesBlockCipher final : public IBlockCipher
    {
    public:
        DesBlockCipher() = default;

        std::string name() const override {
            return "DES";
        }

        bytes_t encrypt(const bytes_t& plaintext) override {
            return encrypt_block(plaintext);
        }

        bytes_t decrypt(const bytes_t& ciphertext) override {
            return decrypt_block(ciphertext);
        }

        void setupKeys(const bytes_t& key) override {
            m_impl.setup_keys(key);
        }

        bytes_t encrypt_block(const bytes_t& block) override {
            return m_impl.encrypt_block(block);
        }

        bytes_t decrypt_block(const bytes_t& block) const override {
            return const_cast<des_cipher&>(m_impl).decrypt_block(block);
        }

        size_t getBlockSize() const override {
            return m_impl.get_block_size();
        }

    private:
        des_cipher m_impl;
    };
}

#endif //CRYPT_DES_HPP
