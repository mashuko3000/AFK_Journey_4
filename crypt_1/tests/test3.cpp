#include <gtest/gtest.h>
#include "../inc/loop.hpp"
#include "../inc/common.hpp"

TEST(BitLoopTest, RotateLeft_Basic) {
auto bits = bits_from_string("10110011");
auto res = rotate_left(bits, 8, 2);
EXPECT_EQ(bits_to_string(res, 8), "11001110");
}

TEST(BitLoopTest, RotateLeft_NonByteMultiple) {
auto bits = bits_from_string("11001");
auto res = rotate_left(bits, 5, 2);
EXPECT_EQ(bits_to_string(res, 5), "00111");
}

TEST(BitLoopTest, RotateLeft_LargeK) {
auto bits = bits_from_string("101");
auto res = rotate_left(bits, 3, 10);
EXPECT_EQ(bits_to_string(res, 3), "011");
}

TEST(BitLoopTest, RotateRight_Basic) {
auto bits = bits_from_string("10110011");
auto res = rotate_right(bits, 8, 2);
EXPECT_EQ(bits_to_string(res, 8), "11101100");
}

TEST(BitLoopTest, RotateRight_CrossByte) {
auto bits = bits_from_string("1000000000000001");
auto res = rotate_right(bits, 16, 1);
EXPECT_EQ(bits_to_string(res, 16), "1100000000000000");
}

TEST(BitLoopTest, ApplyMask_Xor) {
auto bits = bits_from_string("11001101");
auto mask = bits_from_string("11110000");
auto res = apply_mask(bits, mask, 8, mask_operation::Xor);
EXPECT_EQ(bits_to_string(res, 8), "00111101");
}

TEST(BitLoopTest, ApplyMask_And_WithCleanup) {
auto bits = bits_from_string("11111111");
auto mask = bits_from_string("1010");
// Так как n_bits = 8, а маска короче, хвост занулится при AND
auto res = apply_mask(bits, mask, 8, mask_operation::And);
EXPECT_EQ(bits_to_string(res, 8), "10100000");
}

TEST(BitLoopTest, BitSlice_Middle) {
auto bits = bits_from_string("00110110");
auto res = bit_slice(bits, 8, 2, 5);
EXPECT_EQ(bits_to_string(res, 4), "1101");
}

TEST(BitLoopTest, SwapBits_Edge) {
auto bits = bits_from_string("10000001");
auto res = swap_bits(bits, 8, 0, 7);
EXPECT_EQ(bits_to_string(res, 8), "10000001");

res = swap_bits(bits, 8, 0, 1);
EXPECT_EQ(bits_to_string(res, 8), "01000001");
}

TEST(BitLoopTest, SetBit_Modify) {
auto bits = bits_from_string("0000");
auto res = set_bit(bits, 4, 1, true);
EXPECT_EQ(bits_to_string(res, 4), "0100");
res = set_bit(res, 4, 1, false);
EXPECT_EQ(bits_to_string(res, 4), "0000");
}