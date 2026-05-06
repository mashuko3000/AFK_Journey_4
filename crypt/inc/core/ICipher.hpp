//
// Created by maria on 07.04.2026.
//

#ifndef CRYPT_ICIPHER_HPP
#define CRYPT_ICIPHER_HPP

#include"../common/CryptoTypes.hpp"
#include<string>

namespace crypto
{
    class ICipher {
    public:
        virtual ~ICipher() = default;
        [[nodiscard]] virtual bytes_t encrypt(const bytes_t& plaintext) = 0;
        [[nodiscard]] virtual bytes_t decrypt(const bytes_t& ciphertext) = 0;

        [[nodiscard]] virtual std::string name() const = 0;
        [[nodiscard]] virtual bool isSymmetric() const = 0;
        [[nodiscard]] virtual bool isBlockCipher() const {return false;}
    };
}

#endif //CRYPT_ICIPHER_HPP
