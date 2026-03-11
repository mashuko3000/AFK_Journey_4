//
// Created by mvideo on 06/03/26.
//

#ifndef CRYPT_2_CFB_MODE_HPP
#define CRYPT_2_CFB_MODE_HPP

#include"../core_interfaces/i_mode.hpp"

class CFB_mode : public IMode
{
public:
    void encrypt(
            const bytes_t& data,
            bytes_t& cipher,
            i_cipher* algo,
            const bytes_t& iv,
            int threads,
            const AdditionalParams& params = {}
    ) override;
    void decrypt(
            bytes_t& cipher,
            bytes_t& result,
            i_cipher* algo,
            const bytes_t& iv,
            int threads,
            const AdditionalParams& params = {}
    ) override;

private:
    void xor_blocks(bytes_t& target, const bytes_t& mask, size_t len) const;
    void decrypt_sequential(
            const bytes_t& data,
            bytes_t& cipher,
            i_cipher* algo,
            const bytes_t& iv
    );
};


#endif //CRYPT_2_CFB_MODE_HPP
