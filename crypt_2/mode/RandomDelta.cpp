//
// Created by mvideo on 06/03/26.
//

#include "RandomDelta.hpp"

void RandomDelta::next_counter(bytes_t& counter, const bytes_t& delta) const
{
    int carry = 0;
    for (int i = static_cast<int>(counter.size()) - 1; i >= 0; --i)
    {
        int sum = counter[i] + delta[i] + carry;
        counter[i] = static_cast<unsigned char>(sum & 0xFF);
        carry = sum >> 8;
    }
}

bytes_t RandomDelta::get_counter_for_block(const bytes_t& iv, const bytes_t& delta, uint64_t offset) const
{
    bytes_t res = iv;
    for (uint64_t i = 0; i < offset; ++i)
    {
        next_counter(res, delta);
    }
    return res;
}