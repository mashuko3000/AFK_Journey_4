//
// Created by maria on 07.04.2026.
//

#ifndef CRYPT_CRYPTOTYPES_HPP
#define CRYPT_CRYPTOTYPES_HPP

#include<cstdint>
#include<vector>
#include <boost/multiprecision/cpp_int.hpp>

namespace crypto
{
    using byte_t = uint8_t;
    using bytes_t = std::vector<byte_t>;

    using BigInt = boost::multiprecision::cpp_int;
    using bigint = boost::multiprecision::cpp_int;
    class Polynomial{};

}

#endif //CRYPT_CRYPTOTYPES_HPP
