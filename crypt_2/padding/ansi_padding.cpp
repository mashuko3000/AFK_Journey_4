//
// Created by mvideo on 03/03/26.
//

#include "ansi_padding.hpp"
#include<stdexcept>
#include <iostream>

bytes_t AnsiX923::add(bytes_t& data, size_t block_size) const
{
    if(block_size == 0) throw std::invalid_argument("[ANSIX923] [ADD] block size is 0");

    size_t data_size = data.size();
    bytes_t result = data;
    size_t rem = data_size % block_size;
    size_t to_add = block_size - rem;

    for(size_t i = 0; i < to_add - 1; i++)
    {
        result.push_back(0x00);
    }

    result.push_back(static_cast<byte_t>(to_add));

    return result;
}
bytes_t AnsiX923::remove(bytes_t& data, size_t block_size) const
{
    if(data.empty()) return data;
    if(block_size == 0) throw std::invalid_argument("[ANSIX923] [REMOVE] block size is 0");

    size_t data_size = data.size();

    if(data_size % block_size != 0) throw std::invalid_argument("[ANSIX923] [REMOVE] data size is not a multiple of block size");

    byte_t pad_len = data.back();

    if(pad_len > block_size || pad_len == 0x00) throw std::runtime_error("[ANSIX923] [REMOVE] invalid padding length value");

    size_t pad_start = data_size - pad_len;

    for(size_t i = pad_start; i < data_size - 1; ++i)
    {
        if(data[i] != 0x00)
        {
            std::cout << "Error at index " << i << ": expected 00, got " << (int)data[i] << std::endl;
            throw std::invalid_argument("[ANSIX923] [REMOVE] non-zero bytes in ANSI X.923 padding");
        }
    }

    return bytes_t(data.begin(), data.begin() + pad_start);;
}