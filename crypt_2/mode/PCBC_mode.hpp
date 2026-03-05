//
// Created by mvideo on 03/03/26.
//

#ifndef CRYPT_2_PCBC_MODE_HPP
#define CRYPT_2_PCBC_MODE_HPP

#include"../core_interfaces/i_mode.hpp"

class PCBCMode : public IMode
{
public:
    ~PCBCMode() = default;
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
    void xor_blocks(bytes_t& target, const bytes_t& source) const;
};


#endif //CRYPT_2_PCBC_MODE_HPP
