//
// Created by mvideo on 06/03/26.
//

#include "CTR_mode.hpp"
#include<stdexcept>
#include <future>

void CTR_mode::process_range(
        const bytes_t& input,
        bytes_t& output,
        i_cipher* algo,
        const bytes_t& iv,
        size_t start_pos,
        size_t end_pos
) const
{
    size_t block_size = algo->get_block_size();
    uint64_t start_block_idx = start_pos / block_size;

    bytes_t current_counter = get_counter_for_block(iv, {}, start_block_idx);

    for(size_t i = start_pos; i < end_pos; i += block_size)
    {
        bytes_t mask = algo->encrypt_block(current_counter);
        size_t current_chunk = std::min(block_size, end_pos - i);

        for(size_t j = 0; j < current_chunk; ++j)
        {
            output[i + j] = input[i + j] ^ mask[j];
        }
        next_counter(current_counter, {});
    }
}

void CTR_mode::next_counter(bytes_t& counter, const bytes_t& delta) const
{
    increment_block(counter);
}

bytes_t CTR_mode::get_counter_for_block(const bytes_t& iv, const bytes_t& delta, uint64_t offset) const
{
    bytes_t block = iv;
    for(uint64_t i = 0; i < offset; ++i)
    {
        increment_block(block);
    }
    return block;
}

bytes_t CTR_mode::prepare_counter_block(const bytes_t& iv, uint64_t offset) const
{
    bytes_t block = iv;
    for(uint64_t i = 0; i < offset; ++i)
    {
        increment_block(block);
    }
    return block;
}
void CTR_mode::increment_block(bytes_t& block) const
{
    for(int i = static_cast<int>(block.size()) - 1; i >= 0; --i)
    {
        if(++block[i] != 0)
        {
            break;
        }
    }
}

void CTR_mode::encrypt(
        const bytes_t& data,
        bytes_t& cipher,
        i_cipher* algo,
        const bytes_t& iv,
        int threads,
        const AdditionalParams& params
)
{
    if (data.empty())
    {
        cipher.clear();
        return;
    }
    if (!algo)
    {
        throw std::invalid_argument("[CTR][ENC] algorithm pointer is null");
    }

    size_t total_size = data.size();
    size_t block_size = algo->get_block_size();
    cipher.resize(total_size);

    if (threads <= 1 || total_size <= block_size)
    {
        process_range(data, cipher, algo, iv, 0, total_size);
        return;
    }

    size_t num_blocks = (total_size + block_size - 1) / block_size;
    size_t blocks_per_thread = (num_blocks + threads - 1) / threads;
    size_t chunk_size = blocks_per_thread * block_size;

    std::vector<std::future<void>> futures;

    for (size_t start = 0; start < total_size; start += chunk_size)
    {
        size_t end = std::min(start + chunk_size, total_size);

        futures.push_back(std::async(std::launch::async, [this, &data, &cipher, algo, &iv, start, end]()
        {
            this->process_range(data, cipher, algo, iv, start, end);
        }));
    }

    for (auto& f : futures)
    {
        f.get();
    }
}
void CTR_mode::decrypt(
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