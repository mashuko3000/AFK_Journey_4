#ifndef CRYPT_1_P_BOX_HPP
#define CRYPT_1_P_BOX_HPP

#include<vector>
#include<cstdint>
#include<stdexcept>

enum class bit_order
{
    little_endian,
    big_endian
};

enum class bit_index
{
    zero,
    one
};

std::vector<uint8_t> p_box(
        const std::vector<uint8_t> &input,
        const std::vector<size_t> &rule,
        const bit_order order,
        const bit_index index
        );

#endif //CRYPT_1_P_BOX_HPP
