//
// Created by mvideo on 03/03/26.
//

#include "PCBC_mode.hpp"
#include<stdexcept>

void PCBCMode::encrypt(
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
    if(data_size % block_size != 0) throw std::invalid_argument("[PCBC][ENCRYPT] data size error");

    cipher.resize(data_size);

    bytes_t prev_plain(block_size, 0);
    bytes_t prev_cipher = iv.empty()
            ? bytes_t(block_size, 0)
            : iv;
    if(prev_cipher.size() % block_size != 0) throw std::runtime_error("[PCBC][ENCRYPT] iv is incorrect");

    bytes_t mask = prev_cipher;

    size_t block_per_thread = data_size / block_size;

    for(size_t i = 0; i < block_per_thread; ++i)
    {
        size_t shift = i * block_size;
        bytes_t current_plain(data.begin() + shift, data.begin() + shift + block_size);
        bytes_t to_encrypt = current_plain;
        xor_blocks(to_encrypt, mask);
        bytes_t current_cipher = algo->encrypt_block(to_encrypt);
        mask = current_plain;
        xor_blocks(mask, current_cipher);
        std::copy(current_cipher.begin(), current_cipher.end(), cipher.begin() + i * block_size);
    }
}
void PCBCMode::decrypt(
        bytes_t& cipher,
        bytes_t& result,
        i_cipher* algo,
        const bytes_t& iv,
        int threads,
        const AdditionalParams& params
)
{
    size_t block_size = algo->get_block_size();
    size_t data_size = cipher.size();

    if (data_size % block_size != 0) throw std::invalid_argument("[PCBC][decrypted] size error");

    result.resize(data_size);
    bytes_t mask = iv.empty() ? bytes_t(block_size, 0) : iv;
    size_t block_per_thread = data_size / block_size;

    for(size_t i = 0; i < block_per_thread; ++i)
    {
        size_t shift = i * block_size;
        bytes_t current_plain(cipher.begin() + shift, cipher.begin() + shift + block_size);

        bytes_t to_decrypted = algo->decrypt_block(current_plain);
        xor_blocks(to_decrypted, mask);

        mask = current_plain;
        xor_blocks(mask, to_decrypted);

        std::copy(to_decrypted.begin(), to_decrypted.end(), result.begin() + shift);
    }
}

void PCBCMode::xor_blocks(bytes_t& target, const bytes_t& source) const
{
    if(target.empty() || source.empty()) throw std::invalid_argument("[CBC][XOR] block/s is/are empty");
    size_t size = target.size();
    for(size_t i = 0; i < size; ++i)
    {
        target[i]^=source[i];
    }
}
