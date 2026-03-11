//
// Created by mvideo on 06/03/26.
//

#ifndef CRYPT_2_OFB_MODE_HPP
#define CRYPT_2_OFB_MODE_HPP

#include"../core_interfaces/i_mode.hpp"

class OFB_mode final : public IMode
{
public:
    ~OFB_mode() override = default;
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
};


#endif //CRYPT_2_OFB_MODE_HPP
