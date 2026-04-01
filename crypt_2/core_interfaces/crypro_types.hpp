#ifndef CRYPT_2_CRYPRO_TYPES_HPP
#define CRYPT_2_CRYPRO_TYPES_HPP

#include <vector>
#include <cstdint>

using byte_t = std::uint8_t;
using bytes_t = std::vector<std::uint8_t>;
using round_key_t = bytes_t;
using round_keys_t = std::vector<round_key_t>;

#endif //CRYPT_2_CRYPRO_TYPES_HPP
