#include "../../inc/modes/CFB_mode.hpp"
#include <future>

namespace crypto {
    void CFB_mode::xor_blocks(bytes_t &target, const bytes_t &source) const {
        for (size_t i = 0; i < target.size(); ++i) target[i] ^= source[i];
    }

    bytes_t CFB_mode::encrypt(const bytes_t &data, const IBlockCipher *algo, const bytes_t &iv, int threads) const {
        size_t block_size = algo->getBlockSize();
        auto *mutable_algo = const_cast<IBlockCipher *>(algo);
        bytes_t result(data.size());
        bytes_t feedback = iv;

        for (size_t i = 0; i < data.size() / block_size; ++i) {
            bytes_t encrypted_feedback = mutable_algo->encrypt_block(feedback);
            bytes_t block(data.begin() + i * block_size, data.begin() + (i + 1) * block_size);
            xor_blocks(block, encrypted_feedback);

            std::copy(block.begin(), block.end(), result.begin() + i * block_size);
            feedback = block;
        }
        return result;
    }

    void CFB_mode::decrypt_range(const bytes_t &cipher, bytes_t &result, const IBlockCipher *algo, const bytes_t &iv,
                                 size_t start_block, size_t end_block) const {
        size_t block_size = algo->getBlockSize();
        auto *mutable_algo = const_cast<IBlockCipher *>(algo);

        for (size_t i = start_block; i < end_block; ++i) {
            bytes_t prev_cipher_block = (i == 0) ? iv : bytes_t(cipher.begin() + (i - 1) * block_size,
                                                                cipher.begin() + i * block_size);
            bytes_t keystream = mutable_algo->encrypt_block(prev_cipher_block);

            bytes_t current_cipher_block(cipher.begin() + i * block_size, cipher.begin() + (i + 1) * block_size);
            xor_blocks(current_cipher_block, keystream);
            std::copy(current_cipher_block.begin(), current_cipher_block.end(), result.begin() + i * block_size);
        }
    }

    bytes_t CFB_mode::decrypt(const bytes_t &cipher, const IBlockCipher *algo, const bytes_t &iv, int threads) const {
        size_t block_size = algo->getBlockSize();
        size_t num_blocks = cipher.size() / block_size;
        bytes_t result(cipher.size());

        if (threads <= 1) {
            decrypt_range(cipher, result, algo, iv, 0, num_blocks);
        } else {
            std::vector<std::future<void>> futures;
            size_t blocks_per_thread = num_blocks / threads;
            for (int i = 0; i < threads; ++i) {
                size_t start = i * blocks_per_thread;
                size_t end = (i == threads - 1) ? num_blocks : (i + 1) * blocks_per_thread;
                futures.push_back(std::async(std::launch::async, &CFB_mode::decrypt_range, this, std::ref(cipher),
                                             std::ref(result), algo, std::ref(iv), start, end));
            }
        }
        return result;
    }
}