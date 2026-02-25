//
// Created by mvideo on 22/02/26.
//

#ifndef CRYPT_2_TRIPPLE_DES_HPP
#define CRYPT_2_TRIPPLE_DES_HPP

#include "des.hpp"
#include "interfaces.hpp"

enum class triple_des_mode
{
    EEE3,
    EDE3,
    EEE2,
    EDE2,
};

class triple_des final: public i_cipher
{
public:
    explicit triple_des(triple_des_mode m = triple_des_mode::EDE3) : mode(m) {}
private:
    des_cipher des1;
    des_cipher des2;
    des_cipher des3;
    triple_des_mode mode;

public:
    void setup_keys(const bytes_t & key) override;
    bytes_t encrypt_block(const bytes_t & block) override;
    bytes_t decrypt_block(const bytes_t & block) override;
};

#endif //CRYPT_2_TRIPPLE_DES_HPP
