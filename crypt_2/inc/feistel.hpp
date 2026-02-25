#ifndef CRYPT_2_FEISTEL_HPP
#define CRYPT_2_FEISTEL_HPP

#include "interfaces.hpp"
#include <stdexcept>
#include <algorithm>

#define BLOCK_SIZE 8

class feistel_network : public i_cipher
{
private:
    i_key_expander& _expander;
    i_feistel_transform& _transform;
    const int _rounds;

    round_keys_t _enc_sub_keys;
    round_keys_t _dec_sub_keys;

public:
    // rule of 5

    feistel_network(
            i_key_expander& expander,
            i_feistel_transform& transform,
            int rounds);

    virtual ~feistel_network() = default;

    feistel_network(const feistel_network&) = delete;
    feistel_network(feistel_network&&) =delete;

    feistel_network& operator=(const feistel_network&) = delete;
    feistel_network& operator=(feistel_network&&) = delete;

public:
    // i_cipher methods
    void setup_keys(const bytes_t & key) override;

    bytes_t encrypt_block(const bytes_t & block) override;
    bytes_t decrypt_block(const bytes_t & block) override;

private:
    bytes_t encrypt_rounds(const bytes_t& block) const;
    bytes_t decrypt_rounds(const bytes_t& block) const;

    //utilits
    static bytes_t xor_bytes(const bytes_t& a, const bytes_t& b);
};

#endif //CRYPT_2_FEISTEL_HPP
