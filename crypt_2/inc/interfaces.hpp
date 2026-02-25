
#ifndef CRYPT_2_INTERFACES_HPP
#define CRYPT_2_INTERFACES_HPP

#include "crypro_types.hpp"

class i_key_expander
{
public:
    virtual ~i_key_expander() = default;
    virtual round_keys_t expand(const bytes_t& master_key) const = 0;
};

class i_feistel_transform
{
public:
    virtual ~i_feistel_transform() = default;
    virtual bytes_t transform(const bytes_t& block, const round_key_t& round_key) = 0;
};

class i_cipher
{
public:
    virtual ~i_cipher() = default;
    virtual void setup_keys(const bytes_t & key) = 0;

    virtual bytes_t encrypt_block(const bytes_t & block) = 0;
    virtual bytes_t decrypt_block(const bytes_t & block) = 0;
};

#endif //CRYPT_2_INTERFACES_HPP
