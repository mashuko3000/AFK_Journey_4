#include "../inc/p_box.hpp"

std::vector<uint8_t> p_box(
        const std::vector<uint8_t> &input,
        const std::vector<size_t> &rule,
        const bit_order order,
        const bit_index index
)
{
    if(input.empty()) throw std::invalid_argument("Input data vector is empty");
    if(rule.empty()) throw std::invalid_argument("Rule box vector is empty");

    const size_t total_input_bits_count = input.size() << 3;
    const int offset = static_cast<int>(index);

    const size_t output_size = (rule.size() + 7) >> 3;//(rule.size() + 7) / 8; //save bits after division
    std::vector<uint8_t> output(output_size, 0);

    auto get_bit_pos = [order](size_t bit_idx) -> size_t
    {
        const size_t pos_in_byte = bit_idx & 7;
        return(order == bit_order::little_endian) ? pos_in_byte : 7 - pos_in_byte;
    };

    for (size_t i = 0; i < rule.size(); ++i)
    {
        const int src_idx = static_cast<int>(rule[i]) - offset;

        if (src_idx < 0 || static_cast<size_t>(src_idx) >= total_input_bits_count) {
            throw std::out_of_range("P-box index out of range");
        }

        const bool bit_is_set = (input[src_idx >> 3] >> get_bit_pos(src_idx)) & 1;
        if(bit_is_set)
        {
            output[i >> 3] |= (1 << get_bit_pos(i));
        }
    }
    return output;
}