#ifndef CRYPT_1_LOOP_HPP
#define CRYPT_1_LOOP_HPP

#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <vector>

enum class mask_operation {
    And,
    Or,
    Xor
};

std::vector<uint8_t> rotate_left(
        const std::vector<uint8_t>& input,
        size_t n_bits,
        size_t k_shift
        );

std::vector<uint8_t> rotate_right(
        const std::vector<uint8_t>& input,
        const size_t n_bits,
        size_t k_shift
);

std::vector<uint8_t> apply_mask(
        const std::vector<uint8_t>& input,
        const std::vector<uint8_t>& mask,
        const size_t n_bits,
        const mask_operation op = mask_operation::And
);

std::vector<uint8_t> bit_slice(
        const std::vector<uint8_t>& input,
        const size_t n_bits,
        const size_t i,
        const size_t j
);

std::vector<uint8_t> swap_bits(
        const std::vector<uint8_t>& input,
        const size_t n_bits,
        const size_t i,
        const size_t j
);

std::vector<uint8_t> set_bit(
        const std::vector<uint8_t>& input,
        const size_t n_bits,
        const size_t i,
        const bool state
);

void validate_input(
        const std::vector<uint8_t>& input,
        const size_t n_bits
);

void trim_unused_bits(
        std::vector<uint8_t>& result,
        const size_t n_bits
);
#endif //CRYPT_1_LOOP_HPP
