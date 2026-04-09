//
// Created by mvideo on 03/03/26.
//

#include "../inc/modes/CBC_mode.hpp"
#include<stdexcept>
#include<future>

namespace crypto
{
    void CBC_mode::xor_blocks(bytes_t &target, const bytes_t &source) const
    {
        if (target.empty() || source.empty()) throw std::invalid_argument("[CBC][XOR] block/s is/are empty");
        size_t size = target.size();
        for (size_t i = 0; i < size; ++i)
        {
            target[i] ^= source[i];
        }
    }

    void CBC_mode::decrypt_range(const bytes_t& cipher,
                                bytes_t& result,
                                const IBlockCipher* algo,
                                const bytes_t& iv,
                                size_t start_block,
                                size_t end_block) const
    {
        size_t block_size = algo->getBlockSize();

        for (size_t i = start_block; i < end_block; ++i)
        {
            bytes_t current_block(cipher.begin() + i * block_size, cipher.begin() + (i + 1) * block_size);
            bytes_t decrypted = algo->decrypt_block(current_block);

            bytes_t prev_block = (i == 0)
                    ? iv
                    : bytes_t(cipher.begin() + (i - 1) * block_size, cipher.begin() + i * block_size);

            xor_blocks(decrypted, prev_block);
            std::copy(decrypted.begin(), decrypted.end(), result.begin() + i * block_size);
        }
    }

    bytes_t CBC_mode::encrypt(
            const bytes_t &data,
            const IBlockCipher *algo,
            const bytes_t &iv,
            int threads) const
    {
        size_t block_size = algo->getBlockSize();
        size_t data_size = data.size();

        if (data_size % block_size != 0) throw std::invalid_argument("[CBC] [ENCRYPT] data size must be multiple of block size");

        bytes_t cipher(data.size());
        bytes_t prev_block = iv.empty()
                             ? bytes_t(block_size, 0)
                             : iv;

        if (prev_block.size() != block_size)
        {
            throw std::invalid_argument("[CBC][ENCRYPT] IV size must match block size");
        }

        bytes_t block_buffer(block_size);

        for (size_t i = 0; i < data_size / block_size; ++i)
        {
            bytes_t block_buffer(data.begin() + i * block_size, data.begin() + (i + 1) * block_size);
            xor_blocks(block_buffer, prev_block);
            bytes_t encrypted = const_cast<IBlockCipher*>(algo)->encrypt_block(block_buffer);
            std::copy(encrypted.begin(), encrypted.end(), cipher.begin() + i * block_size);
            prev_block = encrypted;
        }
        return cipher;
    }

    bytes_t CBC_mode::decrypt(const bytes_t& cipher,
                    const IBlockCipher* algo,
                    const bytes_t& iv,
                    int threads) const
    {
        size_t block_size = algo->getBlockSize();
        size_t num_blocks = cipher.size() / block_size;
        bytes_t result(cipher.size());

        if (threads <= 1)
        {
            decrypt_range(cipher, result, algo, iv, 0, num_blocks);
        }
        else
        {
            std::vector<std::future<void>> futures;
            size_t blocks_per_thread = num_blocks / threads;
            for (int i = 0; i < threads; ++i)
            {
                size_t start = i * blocks_per_thread;
                size_t end = (i == threads - 1) ? num_blocks : (i + 1) * blocks_per_thread;
                futures.push_back(std::async(std::launch::async, &CBC_mode::decrypt_range,
                                             this, std::ref(cipher), std::ref(result), algo,
                                             std::ref(iv), start, end));
            }
        }
        return result;
    }


}
