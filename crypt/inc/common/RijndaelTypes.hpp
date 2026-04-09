//
// Created by maria on 07.04.2026.
//

#ifndef CRYPT_RIJNDAELTYPES_HPP
#define CRYPT_RIJNDAELTYPES_HPP

#include<cstddef>

namespace crypto
{
    enum class RijndaelBlockSize : size_t {Bits128 = 128, Bits192 = 192, Bits256 = 256};
    enum class RijndaelKeySize : size_t {Bits128 = 128, Bits192 = 192, Bits256 = 256};
}

#endif //CRYPT_RIJNDAELTYPES_HPP
