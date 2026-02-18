#include"../inc/rc4_cipher.hpp"
#include <numeric>
#include <algorithm>

void rc4_cipher::set_key(const std::vector<uint8_t>& key)
{
    if (key.empty()) return;

    s_box.assign(256, 0);
    std::iota(s_box.begin(), s_box.end(), 0);

    uint8_t j = 0;
    for (int i = 0; i < 256; ++i)
    {
        j = (j + s_box[i] + key[i % key.size()]) % 256;
        std::swap(s_box[i], s_box[j]);
    }

    this->i = 0;
    this->j = 0;
}
void rc4_cipher::process(uint8_t* data, size_t data_len)
{
    for (size_t k = 0; k < data_len; ++k)
    {
        i = (i + 1) % 256;
        j = (j + s_box[i]) % 256;

        std::swap(s_box[i], s_box[j]);

        uint8_t t = (s_box[i] + s_box[j]) % 256;
        data[k] ^= s_box[t];
    }
}