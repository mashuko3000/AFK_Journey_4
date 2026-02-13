#include <gtest/gtest.h>
#include "../inc/p_box.hpp"
#include "../inc/common.hpp"

TEST(bits_p_box_test, identity_big_endian_zero_based) {
    auto bits = bits_from_string("1010110000110100");
    std::vector<size_t> rule(16);
    for (size_t i = 0; i < 16; i++) rule[i] = i;

    auto result = p_box(bits, rule, bit_order::big_endian, bit_index::zero);
    EXPECT_EQ(bits, result);
}

TEST(bits_p_box_test, identity_little_endian_zero_based) {
    auto bits = bits_from_string("1010110000110100");
    std::vector<size_t> rule(16);
    for (size_t i = 0; i < 16; i++) rule[i] = i;

    auto result = p_box(bits, rule, bit_order::little_endian, bit_index::zero);
    EXPECT_EQ(bits, result);
}

TEST(bits_p_box_test, identity_big_endian_one_based) {
    auto bits = bits_from_string("1100110010101010");
    std::vector<size_t> rule(16);
    for (size_t i = 0; i < 16; i++) rule[i] = i + 1;
    auto result = p_box(bits, rule, bit_order::big_endian, bit_index::one);
    EXPECT_EQ(bits, result);
}

TEST(bits_p_box_test, repeated_bits_little_endian) {
    auto bits = bits_from_string("10100110");
    auto expected = bits_from_string("00110011");
    std::vector<size_t> rule = {2, 2, 1, 1, 6, 6, 5, 5};

    auto result = p_box(bits, rule, bit_order::little_endian, bit_index::one);
    EXPECT_EQ(expected, result);
}

TEST(bits_p_box_test, partial_selection_big_endian) {
    auto bits = bits_from_string("1111000010101010");
    std::vector<size_t> rule = {0, 1, 2, 3, 4, 5, 6, 7};
    auto expected = bits_from_string("11110000");

    auto result = p_box(bits, rule, bit_order::big_endian, bit_index::zero);
    EXPECT_EQ(expected, result);
}

TEST(bits_p_box_test, complex_shuffle_big_endian) {
    auto bits = bits_from_string("10110000");
    std::vector<size_t> rule = {7, 0, 1, 2};
    auto expected = bits_from_string("0101");

    auto result = p_box(bits, rule, bit_order::big_endian, bit_index::zero);
    EXPECT_EQ(expected, result);
}

TEST(bits_p_box_test, throw_on_out_of_range) {
    auto bits = bits_from_string("1010");
    std::vector<size_t> rule = {10};

    EXPECT_THROW(p_box(bits, rule, bit_order::big_endian, bit_index::zero), std::out_of_range);
}