//
// Created by mvideo on 06/03/26.
//

#include "CFB_mode.hpp"
#include <stdexcept>
#include <future>

void CFB_mode::xor_blocks(bytes_t& target, const bytes_t& mask, size_t len) const
{
    if(target.empty() || mask.empty()) throw std::invalid_argument("[CBC][XOR] block/s is/are empty");
    size_t size = target.size();
    for(size_t i = 0; i < size; ++i)
    {
        target[i]^=mask[i];
    }
}

void CFB_mode::encrypt(
        const bytes_t& data,
        bytes_t& cipher,
        i_cipher* algo,
        const bytes_t& iv,
        int threads,
        const AdditionalParams& params
)
{
    size_t block_size = algo->get_block_size();
    if (iv.size() < block_size) throw std::invalid_argument("[CFB] IV size too small");

    size_t data_size = data.size();
    cipher.resize(data_size);

    bytes_t shift_register = iv;

    for(size_t i = 0; i < data_size; i += block_size)
    {
        bytes_t encrypted_block = algo->encrypt_block(shift_register);
        size_t current_chunk = std::min(block_size, data_size - i);
        for (size_t j = 0; j < current_chunk; ++j)
        {
            cipher[i + j] = data[i + j] ^ encrypted_block[j];
            shift_register[j] = cipher[i + j];
        }
    }
}

void CFB_mode::decrypt_sequential(
        const bytes_t& cipher,
        bytes_t& data,
        i_cipher* algo,
        const bytes_t& iv
        )
{
    size_t block_size = algo->get_block_size();
    if (iv.size() < block_size) throw std::invalid_argument("[CFB] IV size too small");

    size_t cipher_size = cipher.size();

    data.resize(cipher_size);
    bytes_t shift_register = iv;

    for (size_t i = 0; i < cipher_size; i += block_size)
    {
        bytes_t encrypted_block = algo->encrypt_block(shift_register);
        size_t current_chunk = std::min(block_size, cipher.size() - i);

        for (size_t j = 0; j < current_chunk; ++j) {
            data[i + j] = cipher[i + j] ^ encrypted_block[j];
            shift_register[j] = cipher[i + j];
        }
    }
}

void CFB_mode::decrypt(
        bytes_t& cipher,
        bytes_t& result,
        i_cipher* algo,
        const bytes_t& iv,
        int threads,
        const AdditionalParams& params
)
{
    size_t block_size = algo->get_block_size();
    size_t total_size = cipher.size();
    result.resize(total_size);

    if(threads <= 1 || total_size <= block_size)
    {
        return decrypt_sequential(cipher, result, algo, iv);
    }

    size_t num_blocks = (total_size + block_size - 1) / block_size;
    size_t blocks_per_thread = (num_blocks + threads - 1) / threads;
    size_t chunk_size = blocks_per_thread * block_size;

    std::vector<std::future<void>> futures;

    for (size_t t = 0; t < (size_t)threads; ++t)
    {
        size_t start_offset = t * chunk_size;
        if (start_offset >= total_size) break;

        size_t end_offset = std::min(start_offset + chunk_size, total_size);

        futures.push_back(std::async(std::launch::async, [=, &cipher, &result]()
        {
            bytes_t shift_register;
            if (start_offset == 0)
            {
                shift_register = iv;
            }
            else
            {
                shift_register.assign(cipher.begin() + start_offset - block_size,
                                      cipher.begin() + start_offset);
            }
            for (size_t i = start_offset; i < end_offset; i += block_size)
            {
                bytes_t mask = algo->encrypt_block(shift_register);
                size_t current_chunk = std::min(block_size, end_offset - i);
                for (size_t j = 0; j < current_chunk; ++j)
                {
                    result[i + j] = cipher[i + j] ^ mask[j];
                    shift_register[j] = cipher[i + j];
                }
            }
        }));
    }
    for (auto& f : futures) f.wait();
}