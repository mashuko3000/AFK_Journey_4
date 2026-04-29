//
// Created by maria on 07.04.2026.
//

#ifndef CRYPT_IMODE_HPP
#define CRYPT_IMODE_HPP

#include "../common/CryptoTypes.hpp"
#include"IBlockCipher.hpp"

namespace crypto
{
    class IMode
    {
    public:
        virtual ~IMode() = default;
        [[nodiscard]] virtual bytes_t encrypt(const bytes_t& data,
                                              const IBlockCipher* algo,
                                              const bytes_t& iv,
                                              int threads) const = 0;

        [[nodiscard]] virtual bytes_t decrypt(const bytes_t& cipher,
                                              const IBlockCipher* algo,
                                              const bytes_t& iv,
                                              int threads) const = 0;
    };
}

#endif //CRYPT_IMODE_HPP
