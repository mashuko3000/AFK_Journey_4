//
// Created by mvideo on 06/03/26.
//

#include "OFB_mode.hpp"
#include <stdexcept>

void OFB_mode::encrypt(
        const bytes_t& data,
        bytes_t& cipher,
        i_cipher* algo,
        const bytes_t& iv,
        int threads,
        const AdditionalParams& params
)
{
    if(data.empty())
    {
        cipher.clear();
        return;
    }
    if(algo == nullptr)
    {
        throw std::invalid_argument("[OFB][ENC] algorithm pointer is null");
    }
    size_t block_size = algo->get_block_size();
    size_t iv_size = iv.size();
    if(iv.empty() || iv_size < block_size)
    {
        throw std::invalid_argument("[OFB][ENC] IV size (" + std::to_string(iv_size) +
                                    ") is smaller than block size (" + std::to_string(block_size) + ")");
    }

    size_t data_size = data.size();
    cipher.resize(data_size);

    bytes_t current_mask(iv.begin(), iv.begin() + block_size);

    for(size_t i = 0; i < data_size; i+=block_size)
    {
        current_mask = algo->encrypt_block(current_mask);
        size_t current_chunk = std::min(block_size, data_size - i);
        for(size_t j = 0; j < current_chunk; ++j)
        {
            cipher[i + j] = data[i + j] ^ current_mask[j];
        }
    }
}
void OFB_mode::decrypt(
        bytes_t& cipher,
        bytes_t& result,
        i_cipher* algo,
        const bytes_t& iv,
        int threads,
        const AdditionalParams& params
)
{
    encrypt(cipher, result, algo, iv, threads, params);
}