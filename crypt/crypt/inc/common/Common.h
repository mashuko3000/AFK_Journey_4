//
// Created by maria on 07.04.2026.
//

#ifndef CRYPT_COMMON_H
#define CRYPT_COMMON_H

#include<cstdint>
#include<array>

namespace crypto
{
    using Byte = uint8_t;
    using SBoxTable = std::array<Byte, 256>;

    template<typename T>
    struct ExtendedGCDResult
    {
        T gcd;
        T s;
        T t;
    };
}

#endif //CRYPT_COMMON_H
