//
// Created by mvideo on 14/02/26.
//

#ifndef CRYPT_1_RC4_CIPHER_HPP
#define CRYPT_1_RC4_CIPHER_HPP

#include <vector>
#include <cstdint>

class rc4_cipher final{
public:
    rc4_cipher() = default;

    void set_key(const std::vector<uint8_t>& key);
    void process(uint8_t* data, size_t data_len);
private:
    std::vector<uint8_t> s_box;
    uint8_t i = 0;
    uint8_t j = 0;
};

#endif //CRYPT_1_RC4_CIPHER_HPP
