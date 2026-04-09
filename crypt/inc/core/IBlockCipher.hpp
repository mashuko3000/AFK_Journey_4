//
// Created by maria on 07.04.2026.
//

#ifndef CRYPT_IBLOCKCIPHER_HPP
#define CRYPT_IBLOCKCIPHER_HPP

#include "core/ICipher.hpp"

namespace crypto
{
    class IBlockCipher : public ICipher
    {
    public:
        virtual ~IBlockCipher() = default;

        [[nodiscard]] virtual bool isSymmetric() const override final {return true;}
        [[nodiscard]] virtual bool isBlockCipher() const override final {return true;}

        [[nodiscard]] virtual bytes_t encrypt_block(const bytes_t& block) = 0;
        [[nodiscard]] virtual bytes_t decrypt_block(const bytes_t& block) const = 0;

        virtual void setupKeys(const bytes_t& key) = 0;
        [[nodiscard]] virtual size_t getBlockSize() const = 0;

    };
}

#endif //CRYPT_IBLOCKCIPHER_HPP
