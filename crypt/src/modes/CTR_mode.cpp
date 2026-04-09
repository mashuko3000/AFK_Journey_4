#include "../../inc/modes/CTR_mode.hpp"
#include <stdexcept>
#include <future>
#include <vector>
#include <algorithm>

namespace crypto {

    void CTR_mode::next_counter(bytes_t &counter) const {
        for (int i = static_cast<int>(counter.size()) - 1; i >= 0; --i) {
            if (++counter[i] != 0) break;
        }
    }

    bytes_t CTR_mode::get_counter_for_block(const bytes_t &iv, uint64_t block_idx) const {
        bytes_t block = iv;
        for (uint64_t i = 0; i < block_idx; ++i) {
            next_counter(block);
        }
        return block;
    }

    void CTR_mode::process_range(const bytes_t &input, bytes_t &output, const IBlockCipher *algo, const bytes_t &iv,
                                 size_t start_pos, size_t end_pos) const {
        size_t block_size = algo->getBlockSize();
        uint64_t start_block_idx = start_pos / block_size;

        bytes_t current_counter = get_counter_for_block(iv, start_block_idx);
        auto *mutable_algo = const_cast<IBlockCipher *>(algo);

        for (size_t i = start_pos; i < end_pos; i += block_size) {
            bytes_t mask = mutable_algo->encrypt_block(current_counter);
            size_t current_chunk = std::min(block_size, end_pos - i);

            for (size_t j = 0; j < current_chunk; ++j) {
                output[i + j] = input[i + j] ^ mask[j];
            }
            next_counter(current_counter);
        }
    }

    bytes_t CTR_mode::encrypt(const bytes_t &data, const IBlockCipher *algo, const bytes_t &iv, int threads) const {
        if (data.empty()) return {};
        if (!algo) throw std::invalid_argument("[CTR] algorithm pointer is null");

        size_t total_size = data.size();
        size_t block_size = algo->getBlockSize();
        bytes_t cipher(total_size);

        if (threads <= 1 || total_size <= block_size) {
            process_range(data, cipher, algo, iv, 0, total_size);
            return cipher;
        }

        size_t num_blocks = (total_size + block_size - 1) / block_size;
        size_t blocks_per_thread = (num_blocks + threads - 1) / threads;
        size_t chunk_size = blocks_per_thread * block_size;

        std::vector<std::future<void>> futures;

        for (size_t start = 0; start < total_size; start += chunk_size) {
            size_t end = std::min(start + chunk_size, total_size);
            futures.push_back(std::async(std::launch::async, [this, &data, &cipher, algo, &iv, start, end]() {
                this->process_range(data, cipher, algo, iv, start, end);
            }));
        }

        for (auto &f: futures) f.get();
        return cipher;
    }

    bytes_t CTR_mode::decrypt(const bytes_t &cipher, const IBlockCipher *algo, const bytes_t &iv, int threads) const {
        return encrypt(cipher, algo, iv, threads);
    }

}