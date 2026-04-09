#include "../../inc/modes/ECB_mode.hpp"

namespace crypto {
    void ECB_mode::process_blocks(const bytes_t &input, bytes_t &output, const IBlockCipher *algo, size_t start_block,
                                  size_t end_block, bool encrypt) const {
        size_t block_size = algo->getBlockSize();
        auto *mutable_algo = const_cast<IBlockCipher *>(algo);

        for (size_t i = start_block; i < end_block; ++i) {
            bytes_t block(input.begin() + i * block_size, input.begin() + (i + 1) * block_size);
            bytes_t processed = encrypt ? mutable_algo->encrypt_block(block) : mutable_algo->decrypt_block(block);
            std::copy(processed.begin(), processed.end(), output.begin() + i * block_size);
        }
    }

    bytes_t ECB_mode::encrypt(const bytes_t &data, const IBlockCipher *algo, const bytes_t &iv, int threads) const {
        size_t block_size = algo->getBlockSize();
        size_t num_blocks = data.size() / block_size;
        bytes_t result(data.size());

        if (threads <= 1) {
            process_blocks(data, result, algo, 0, num_blocks, true);
        } else {
            std::vector<std::future<void>> futures;
            size_t blocks_per_thread = num_blocks / threads;
            for (int i = 0; i < threads; ++i) {
                size_t start = i * blocks_per_thread;
                size_t end = (i == threads - 1) ? num_blocks : (i + 1) * blocks_per_thread;
                futures.push_back(std::async(std::launch::async, &ECB_mode::process_blocks, this, std::ref(data),
                                             std::ref(result), algo, start, end, true));
            }
        }
        return result;
    }

    bytes_t ECB_mode::decrypt(const bytes_t &cipher, const IBlockCipher *algo, const bytes_t &iv, int threads) const {
        size_t block_size = algo->getBlockSize();
        size_t num_blocks = cipher.size() / block_size;
        bytes_t result(cipher.size());

        if (threads <= 1) {
            process_blocks(cipher, result, algo, 0, num_blocks, false);
        } else {
            std::vector<std::future<void>> futures;
            size_t blocks_per_thread = num_blocks / threads;
            for (int i = 0; i < threads; ++i) {
                size_t start = i * blocks_per_thread;
                size_t end = (i == threads - 1) ? num_blocks : (i + 1) * blocks_per_thread;
                futures.push_back(std::async(std::launch::async, &ECB_mode::process_blocks, this, std::ref(cipher),
                                             std::ref(result), algo, start, end, false));
            }
        }
        return result;
    }
}