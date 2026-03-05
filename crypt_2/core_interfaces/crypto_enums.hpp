
#ifndef CRYPT_2_CRYPTO_ENUMS_HPP
#define CRYPT_2_CRYPTO_ENUMS_HPP

#include<vector>
#include <cstdint>

enum class CipherMode {ECB, CBC, PCBC};
enum class PaddingType {Zeros, AnsiX923};
struct AdditionalParams
{
    std::vector<uint8_t> data{};
};

#endif //CRYPT_2_CRYPTO_ENUMS_HPP
