#include "../../inc/modes/OFB_mode.hpp"

namespace crypto {
    bytes_t OFB_mode::encrypt(const bytes_t &data, const IBlockCipher *algo, const bytes_t &iv, int threads) const {
        size_t block_size = algo->getBlockSize();
        auto *mutable_algo = const_cast<IBlockCipher *>(algo);
        bytes_t result(data.size());
        bytes_t keystream_block = iv;

        for (size_t i = 0; i < data.size() / block_size; ++i) {
            keystream_block = mutable_algo->encrypt_block(keystream_block);

            for (size_t j = 0; j < block_size; ++j) {
                result[i * block_size + j] = data[i * block_size + j] ^ keystream_block[j];
            }
        }
        return result;
    }
}