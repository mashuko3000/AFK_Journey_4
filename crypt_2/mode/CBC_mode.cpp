//
// Created by mvideo on 03/03/26.
//

#include "CBC_mode.hpp"
#include<stdexcept>
#include<future>

void CBC_mode::xor_blocks(bytes_t& target, const bytes_t& source) const
{
    if(target.empty() || source.empty()) throw std::invalid_argument("[CBC][XOR] block/s is/are empty");
    size_t size = target.size();
    for(size_t i = 0; i < size; ++i)
    {
        target[i]^=source[i];
    }
}

void CBC_mode::encrypt(
        const bytes_t& data,
        bytes_t& cipher,
        i_cipher* algo,
        const bytes_t& iv,
        int threads,
        const AdditionalParams& params
)
{
    size_t block_size = algo->get_block_size();
    size_t data_size = data.size();
    if(data_size % block_size != 0) throw std::invalid_argument("[CBC] [ENCRYPT] data size must be multiple of block size");
    cipher.resize(data_size);

    bytes_t prev_block = iv.empty()
            ? bytes_t(block_size, 0)
            : iv;

    if(prev_block.size() != block_size)
    {
        throw std::invalid_argument("[CBC][ENCRYPT] IV size must match block size");
    }

    bytes_t block_buffer(block_size);
    size_t count = data_size / block_size;

    for (size_t i = 0; i < count; ++i)
    {
        std::copy(data.begin() + i * block_size,
                  data.begin() + (i + 1) * block_size,
                  block_buffer.begin());
        xor_blocks(block_buffer, prev_block);
        bytes_t encrypted = algo->encrypt_block(block_buffer);
        std::copy(encrypted.begin(), encrypted.end(), cipher.begin() + i * block_size);
        prev_block = encrypted;
    }
}
void CBC_mode::decrypt(
        bytes_t& cipher,
        bytes_t& result,
        i_cipher* algo,
        const bytes_t& iv,
        int threads,
        const AdditionalParams& params
)
{
    size_t block_size = algo->get_block_size();
    size_t cipher_size = cipher.size();
    if(cipher_size % block_size != 0) throw std::invalid_argument("[CBC] [ENCRYPT] data size must be multiple of block size");
    result.resize(cipher_size);

    bytes_t prev_block = iv.empty()
                         ? bytes_t(block_size, 0)
                         : iv;

    if(prev_block.size() != block_size)
    {
        throw std::invalid_argument("[CBC][ENCRYPT] IV size must match block size");
    }

    size_t total_blocks = cipher_size / block_size;

    if (threads < 1) threads = 1;

    if(total_blocks < (size_t)threads) threads = (int)total_blocks;

    std::vector<std::future<void>> futures;

    size_t block_per_thread = total_blocks / threads;

    for (size_t i = 0; i < threads; ++i)
    {
        size_t start = i * block_per_thread;
        size_t end = (i == threads - 1) ? total_blocks : block_per_thread * (i + 1);

        futures.emplace_back(std::async(std::launch::async,
                                        [=, &cipher, &result, &iv]()
        {
            bytes_t current_block(block_size);
            for(size_t j = start; j < end; ++j) {
                std::copy(cipher.begin() + j * block_size,
                          cipher.begin() + (j + 1) * block_size,
                          current_block.begin());

                bytes_t decrypted = algo->decrypt_block(current_block);

                if(j == 0) {
                    xor_blocks(decrypted, iv);
                } else {
                    for (size_t b = 0; b < block_size; ++b) {
                        decrypted[b] ^= cipher[(j - 1) * block_size + b];
                    }
                }
                std::copy(decrypted.begin(), decrypted.end(), result.begin() + j * block_size);
            }
        }));
    }
    for(auto&f : futures) f.get();
}

