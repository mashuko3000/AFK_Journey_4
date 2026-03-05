//
// Created by mvideo on 03/03/26.
//

#include "zeros_padding.hpp"
#include<stdexcept>

bytes_t ZerosPadding::add(bytes_t& data, size_t block_size) const
{
    if(data.empty()) throw std::invalid_argument("[ADD PADDING ZEROES] data is empty");
    if(block_size == 0) throw std::invalid_argument("[PADDING ZEROES] block size must be more than 0");
    bytes_t result = data;
    size_t rem = result.size() % block_size;
    if(rem != 0)
    {
        size_t to_add = block_size - rem;
        for(size_t i = 0; i < to_add; ++i)
        {
            result.push_back(0x00);
        }
    }
    return result;
}
bytes_t ZerosPadding::remove(bytes_t& data, size_t block_size) const
{
    if(data.empty()) throw std::invalid_argument("[REMOVE PADDING ZEROES] data is empty");
    bytes_t result = data;
    while(!result.empty() && result.back() == 0x00)
    {
        result.pop_back();
    }
    return result;
}

