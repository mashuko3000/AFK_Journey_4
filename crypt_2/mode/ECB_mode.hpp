//
// Created by mvideo on 03/03/26.
//

#ifndef CRYPT_2_ECB_MODE_HPP
#define CRYPT_2_ECB_MODE_HPP

#include"../core_interfaces/i_mode.hpp"

class ECBMode : public IMode
{
    ~ECBMode() = default;
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
    bytes_t process(
            const bytes_t& input,
            i_cipher* algo,
            int threads,
            bool encrypt
            );
};


#endif //CRYPT_2_ECB_MODE_HPP
