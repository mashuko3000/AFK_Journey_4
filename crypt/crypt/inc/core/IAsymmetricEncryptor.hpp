//
// Created by maria on 18.04.2026.
//

#ifndef CRYPT_IASYMMETRICENCRYPTOR_HPP
#define CRYPT_IASYMMETRICENCRYPTOR_HPP

#include "../common/CryptoTypes.hpp"
#include <string>

namespace crypto
{
    class IAsymmetricEncryptor
    {
    public:
        virtual ~IAsymmetricEncryptor() = default;
        [[nodiscard]] virtual std::string name() const = 0;

        [[nodiscard]] virtual bytes_t encrypt(const bytes_t& plaintext) = 0;
        [[nodiscard]] virtual bytes_t decrypt(const bytes_t& ciphertext) = 0;

        virtual void generateKeyPair() = 0;
    };
}

#endif //CRYPT_IASYMMETRICENCRYPTOR_HPP
