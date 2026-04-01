
#ifndef CRYPT_2_CRYPTO_ENUMS_HPP
#define CRYPT_2_CRYPTO_ENUMS_HPP

#include<vector>
#include <cstdint>

#include "boost/multiprecision/cpp_int.hpp"

using bigint = boost::multiprecision::cpp_int;

enum class CipherMode {ECB, CBC, PCBC};
enum class PaddingType {Zeros, AnsiX923, PCKS7, ISO10126};
struct AdditionalParams
{
    std::vector<uint8_t> data{};
};

enum class Direction : uint8_t
{
    LEFT = 0,
    RIGHT = 1
};

using Path = std::vector<Direction>;

struct Fraction final
{
    bigint numerator;
    bigint denominator;

    [[nodiscard]] bool operator==(const Fraction& other) const noexcept
    {
        return numerator == other.numerator && denominator == other.denominator;
    }

    [[nodiscard]] bool operator<(const Fraction& other) const noexcept
    {
        return numerator * other.denominator < other.numerator * denominator;
    }

    [[nodiscard]] bool operator>(const Fraction& other) const noexcept
    {
        return numerator * other.denominator > other.numerator * denominator;
    }
};


#endif //CRYPT_2_CRYPTO_ENUMS_HPP
