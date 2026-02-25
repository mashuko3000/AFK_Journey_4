//
// Created by mvideo on 22/02/26.
//

#ifndef CRYPT_2_FEISTEL_WITH_HOOKS_HPP
#define CRYPT_2_FEISTEL_WITH_HOOKS_HPP

#include "feistel.hpp"
class feistel_with_hooks : public i_cipher
{
private:
    feistel_network _feistel;
public:
    feistel_with_hooks(i_key_expander& key_expander,
                           i_feistel_transform& transform,
                           int rounds)
                           : _feistel(key_expander,
                                      transform,
                                      rounds) {};
    
    virtual ~feistel_with_hooks() = default;

    feistel_with_hooks(const feistel_with_hooks&) = delete;
    feistel_with_hooks& operator=(const feistel_with_hooks&) = delete;

    feistel_with_hooks(feistel_with_hooks&&) = delete;
    feistel_with_hooks& operator=(feistel_with_hooks&&) = delete;

public:
    void setup_keys(const bytes_t & key) override final
    {
        prepare_key(key);
        _feistel.setup_keys(key);
        post_key_setup();
    }
    bytes_t encrypt_block(const bytes_t & block) override final
    {
        bytes_t data = pre_encrypt(block);
        bytes_t encrypted = _feistel.encrypt_block(data);
        return post_encrypt(encrypted);
    }
    bytes_t decrypt_block(const bytes_t & block) override final
    {
        bytes_t data = pre_decrypt(block);
        bytes_t decrypted = _feistel.decrypt_block(data);
        return post_decrypt(decrypted);
    }
private:
    //hooks
    virtual void prepare_key(const bytes_t& key) {};
    virtual void post_key_setup() {};
    virtual bytes_t pre_encrypt(const bytes_t& block) const {return block;}
    virtual bytes_t post_encrypt(const bytes_t& block) const {return block;}
    virtual bytes_t pre_decrypt(const bytes_t& block) const {return block;}
    virtual bytes_t post_decrypt(const bytes_t& block) const {return block;}
};

#endif //CRYPT_2_FEISTEL_WITH_HOOKS_HPP
