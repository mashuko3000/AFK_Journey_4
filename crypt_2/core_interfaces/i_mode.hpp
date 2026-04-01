
#ifndef CRYPT_2_I_MODE_HPP
#define CRYPT_2_I_MODE_HPP

#include"crypro_types.hpp"
#include"interfaces.hpp"
#include "crypto_enums.hpp"

class IMode
{
public:
    virtual ~IMode() = default;
    virtual void encrypt(
            const bytes_t& data,
            bytes_t& cipher,
            i_cipher* algo,
             const bytes_t& iv,
             int threads,
             const AdditionalParams& params = {}
             ) = 0;
    virtual void decrypt(
            bytes_t& cipher,
            bytes_t& result,
            i_cipher* algo,
            const bytes_t& iv,
            int threads,
            const AdditionalParams& params = {}
            ) = 0;
};

#endif //CRYPT_2_I_MODE_HPP
