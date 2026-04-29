#include "../../inc/modes/PCBC_mode.hpp"
#include <stdexcept>
#include <algorithm>

namespace crypto {

    void PCBC_mode::xor_blocks(bytes_t &target, const bytes_t &source) const {
        if (target.empty() || source.empty()) throw std::invalid_argument("[PCBC][XOR] block/s is/are empty");
        for (size_t i = 0; i < target.size(); ++i) {
            target[i] ^= source[i];
        }
    }

    bytes_t
    PCBC_mode::encrypt(const bytes_t &data, const IBlockCipher *algo, const bytes_t &iv, int threads) const {
        size_t block_size = algo->getBlockSize();
        size_t data_size = data.size();
        if (data_size % block_size != 0) throw std::invalid_argument("[PCBC][ENCRYPT] data size error");

        bytes_t cipher(data_size);
        bytes_t mask = iv.empty() ? bytes_t(block_size, 0) : iv;

        if (mask.size() != block_size) throw std::runtime_error("[PCBC][ENCRYPT] iv is incorrect");

        auto *mutable_algo = const_cast<IBlockCipher *>(algo);
        size_t total_blocks = data_size / block_size;

        for (size_t i = 0; i < total_blocks; ++i) {
            size_t shift = i * block_size;
            bytes_t current_plain(data.begin() + shift, data.begin() + shift + block_size);

            bytes_t to_encrypt = current_plain;
            xor_blocks(to_encrypt, mask);

            bytes_t current_cipher = mutable_algo->encrypt_block(to_encrypt);

            mask = current_plain;
            xor_blocks(mask, current_cipher);

            std::copy(current_cipher.begin(), current_cipher.end(), cipher.begin() + shift);
        }
        return cipher;
    }

    bytes_t
    PCBC_mode::decrypt(const bytes_t &cipher, const IBlockCipher *algo, const bytes_t &iv, int threads) const {
        size_t block_size = algo->getBlockSize();
        size_t data_size = cipher.size();

        if (data_size % block_size != 0) throw std::invalid_argument("[PCBC][DECRYPT] size error");

        bytes_t result(data_size);
        bytes_t mask = iv.empty() ? bytes_t(block_size, 0) : iv;
        size_t total_blocks = data_size / block_size;

        for (size_t i = 0; i < total_blocks; ++i) {
            size_t shift = i * block_size;
            bytes_t current_cipher(cipher.begin() + shift, cipher.begin() + shift + block_size);

            bytes_t to_decrypted = algo->decrypt_block(current_cipher);
            xor_blocks(to_decrypted, mask);

            mask = current_cipher;
            xor_blocks(mask, to_decrypted);

            std::copy(to_decrypted.begin(), to_decrypted.end(), result.begin() + shift);
        }
        return result;
    }

}