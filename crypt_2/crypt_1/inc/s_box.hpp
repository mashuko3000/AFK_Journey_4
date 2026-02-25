#ifndef CRYPT_1_S_BOX_HPP
#define CRYPT_1_S_BOX_HPP

#include <map>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <functional>
#include "p_box.hpp"

std::vector<uint8_t> map_s_box(
        const std::vector<uint8_t>& input,
        const std::map<uint8_t, uint8_t>& s_map,
        size_t b_in,
        size_t b_out,
        bit_order order
);

std::vector<uint8_t> function_s_box(
        const std::vector<uint8_t>& input,
        const std::function<uint8_t(uint8_t)>& s_func,
        size_t b_in,
        size_t b_out,
        bit_order order
        );

std::vector<uint8_t> s_box(
        const std::vector<uint8_t> &input,
        const std::vector<uint8_t> &lut,
        size_t b_in,
        size_t b_out,
        const bit_order order
        );

#endif //CRYPT_1_S_BOX_HPP
