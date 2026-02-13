#include "../inc/s_box.hpp"

std::vector<uint8_t> map_s_box(
        const std::vector<uint8_t>& input,
        const std::map<uint8_t, uint8_t>& s_map,
        size_t b_in,
        size_t b_out,
        bit_order order
)
{
    std::vector<uint8_t> lut(1<<b_in, 0);
    for(auto const& [key, val] : s_map)
    {
        if (key < lut.size()) lut[key] = val;
    }
    return s_box(input, lut, b_in, b_out, order);
}

std::vector<uint8_t> function_s_box(
        const std::vector<uint8_t>& input,
        const std::function<uint8_t(uint8_t)>& s_func,
        size_t b_in,
        size_t b_out,
        bit_order order
)
{
    std::vector<uint8_t> lut(1 << b_in);
    for (size_t i = 0; i < lut.size(); ++i)
    {
        lut[i] = s_func(static_cast<uint8_t>(i));
    }
    return s_box(input, lut, b_in, b_out, order);
}

std::vector<uint8_t> s_box(
        const std::vector<uint8_t> &input,
        const std::vector<uint8_t> &lut,
        size_t b_in,
        size_t b_out,
        const bit_order order
)
{
    const size_t total_input_bits_count = input.size() << 3;
    const size_t num_blocks = total_input_bits_count / b_in;
    const size_t total_bits_out = num_blocks * b_out;

    std::vector<uint8_t> output((total_bits_out + 7) >> 3, 0);

    auto get_bit_pos = [order](size_t bit_idx) -> size_t
    {
        const size_t pos_in_byte = bit_idx & 7;
        return(order == bit_order::little_endian) ? pos_in_byte : 7 - pos_in_byte;
    };

    size_t out_bit_idx = 0;
    for (size_t b = 0; b < num_blocks; ++b) {
        uint8_t key = 0;
        for (size_t i = 0; i < b_in; ++i) {
            const size_t current_bit = b * b_in + i;
            key = (key << 1) | ((input[current_bit >> 3] >> get_bit_pos(current_bit)) & 1);
        }

        const uint8_t val = lut[key];

        for (size_t i = 0; i < b_out; ++i) {
            if ((val >> (b_out - 1 - i)) & 1) {
                output[out_bit_idx >> 3] |= (1 << get_bit_pos(out_bit_idx));
            }
            out_bit_idx++;
        }
    }
    return output;
}