#include <gtest/gtest.h>
#include "../inc/s_box.hpp"
#include "../inc/common.hpp"

TEST(SBoxPainPoints, Compression_4to2) {
    std::map<uint8_t, uint8_t> s_map = {{0xC, 0x1}, {0xA, 0x2}};
    std::vector<uint8_t> input = bits_from_string("11001010"); // 0xCA

    auto out = map_s_box(input, s_map, 4, 2, bit_order::big_endian);

    EXPECT_EQ(bits_to_string(out).substr(0, 4), "0110");
}

TEST(SBoxPainPoints, Expansion_2to4) {
    std::map<uint8_t, uint8_t> s_map = {{0x1, 0xF}, {0x2, 0x0}};
    std::vector<uint8_t> input = bits_from_string("01100110"); // 0x66

    auto out = map_s_box(input, s_map, 2, 4, bit_order::big_endian);

    EXPECT_EQ(bits_to_string(out), "1111000011110000");
}

TEST(SBoxPainPoints, NonMultiple_3bits) {
    auto s_func = [](uint8_t x) -> uint8_t { return (x == 0b010) ? 1 : 0; };
    std::vector<uint8_t> input = bits_from_string("01010100"); // 8 бит

    auto out = function_s_box(input, s_func, 3, 1, bit_order::big_endian);

    EXPECT_EQ(bits_to_string(out).substr(0, 2), "10");
}

TEST(SBoxPainPoints, DirtyLutValues) {
    auto s_func = [](uint8_t x) -> uint8_t { return 0xFF; };
    std::vector<uint8_t> input = bits_from_string("00000000");

    auto out = function_s_box(input, s_func, 4, 2, bit_order::big_endian);

    EXPECT_EQ(bits_to_string(out).substr(0, 4), "1111");
}

TEST(SBoxPainPoints, LargeInput) {
    std::vector<uint8_t> input(1000, 0xAA); // 1000 байт
    std::map<uint8_t, uint8_t> s_map = {{0xAA, 0x55}};
    auto out = map_s_box(input, s_map, 8, 8, bit_order::big_endian);
    EXPECT_EQ(out.size(), 1000);
    EXPECT_EQ(out[999], 0x55);
}

