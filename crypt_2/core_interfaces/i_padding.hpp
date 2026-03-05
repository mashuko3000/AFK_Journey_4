
#ifndef CRYPT_2_I_PADDING_HPP
#define CRYPT_2_I_PADDING_HPP

#include"crypro_types.hpp"

class IPadding
{
public:
    virtual ~IPadding() = default;
    virtual bytes_t add(bytes_t& data, size_t block_size) const = 0;
    virtual bytes_t remove(bytes_t& data, size_t block_size) const = 0;
};

#endif //CRYPT_2_I_PADDING_HPP
