//
// Created by mvideo on 06/03/26.
//

#include "pcks7_padding.hpp"
#include<stdexcept>

bytes_t PCKS7::add(bytes_t& data, size_t block_size) const
{
    if(block_size <= 0 || block_size > 256) throw std::invalid_argument("[PCKS7][ADD] invalid block size");
    if(data.empty()) throw std::invalid_argument("[PCKS7][ADD] empty data");

    size_t data_size = data.size();
    size_t to_add = block_size - (data_size % block_size);

    byte_t add_value = static_cast<byte_t>(to_add);
    bytes_t result = data;

    for(size_t i = 0; i < to_add; ++i)
    {
        result.push_back(add_value);
    }

    return result;
}

bytes_t PCKS7::remove(bytes_t& data, size_t block_size) const
{
    if(block_size <= 0 || block_size > 256) throw std::invalid_argument("[PCKS7][REMOVE] invalid block size");
    if(data.empty()) throw std::invalid_argument("[PCKS7][REMOVE] empty data");

    size_t data_size = data.size();

    if(data_size % block_size != 0) throw std::invalid_argument("[PCKS7][REMOVE] data size not multiple of block size");

    byte_t last_block = data.back();

    if(last_block == 0 || last_block > block_size) throw std::runtime_error("[PCKS7][REMOVE] invalid padding length byte");

    for(size_t i = data_size - last_block; i < data_size; ++i)
    {
        if (data[i] != last_block) throw std::runtime_error("[PCKS7][REMOVE] integrity check failed: padding bytes mismatch");
    }
    size_t original_size = data_size - last_block;
    return bytes_t(data.begin(), data.begin() + original_size);
}
 // PCKS7