//
// Created by mvideo on 03/03/26.
//

#include "ECB_mode.hpp"
#include<stdexcept>
#include<future>

void ECBMode::encrypt(
        const bytes_t& data,
        bytes_t& cipher,
        i_cipher* algo,
        const bytes_t& iv,
        int threads,
        const AdditionalParams& params
)
{
    cipher = process(data, algo, threads, true);
}
void ECBMode::decrypt(
        bytes_t& cipher,
        bytes_t& result,
        i_cipher* algo,
        const bytes_t& iv,
        int threads,
        const AdditionalParams& params
)
{
    result = process(cipher, algo, threads, false);
}

bytes_t ECBMode::process(
        const bytes_t& input,
        i_cipher* algo,
        int threads,
        bool encrypt
)
{
    size_t block_size = algo->get_block_size();
    size_t total_blocks = input.size() / block_size;

    if(threads < 1) threads = 1;
    if(total_blocks < (size_t)threads) threads = (int)total_blocks;

    std::vector<std::future<bytes_t>> futures;
    size_t block_per_thread = total_blocks/threads;

    for(int t = 0; t < threads; ++t)
    {
        size_t start_block = t * block_per_thread;
        size_t end_block = (t == threads - 1)
                ? total_blocks
                : (t + 1) * block_per_thread;

        futures.push_back(std::async(std::launch::async, [=, &input]()
        {
            bytes_t local_result;
            local_result.reserve((end_block - start_block) * block_size);
            for(size_t i = start_block; i < end_block; ++i)
            {
                bytes_t block(input.begin() + i * block_size,
                              input.begin() + (i+1)* block_size);
                bytes_t processed = encrypt
                        ? algo->encrypt_block(block)
                        : algo->decrypt_block(block);
                local_result.insert(local_result.end(), processed.begin(), processed.end());
            }
            return local_result;
        }));
    }
    bytes_t final_result;
    final_result.reserve(input.size());
    for(auto& p : futures)
    {
        bytes_t part = p.get();
        final_result.insert(final_result.end(), part.begin(), part.end());
    }


    /*
    bytes_t result.reserve(input.size());
    std::vector<std::thread> workers;
    size_t blocks_per_thread = total_blocks / threads;

    for (int t = 0; t < threads; ++t) {
        size_t start = t * blocks_per_thread;
        size_t end = (t == threads - 1) ? total_blocks : (t + 1) * blocks_per_thread;

        workers.emplace_back([&, start, end]() {
            bytes_t buffer(block_size);
            for (size_t i = start; i < end; ++i) {
                std::copy(input.begin() + i * block_size,
                          input.begin() + (i + 1) * block_size,
                          buffer.begin());

                bytes_t processed = encrypt ? algo->encrypt_block(buffer)
                                            : algo->decrypt_block(buffer);
                std::copy(processed.begin(), processed.end(), result.begin() + i * block_size);
            }
        });
    }
    for (auto& w : workers) w.join();
     */
    return final_result;
}