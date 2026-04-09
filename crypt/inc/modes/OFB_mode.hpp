//
// Created by mvideo on 06/03/26.
//

#ifndef CRYPT_2_OFB_MODE_HPP
#define CRYPT_2_OFB_MODE_HPP

#include"../core/IMode.hpp"

namespace crypto
{
    class OFB_mode : public IMode
    {
    public:
        bytes_t encrypt(const bytes_t& data,
                        const IBlockCipher* algo,
                        const bytes_t& iv,
                        int threads) const override;

        bytes_t decrypt(const bytes_t& cipher,
                        const IBlockCipher* algo,
                        const bytes_t& iv,
                        int threads) const override
        {
            return encrypt(cipher, algo, iv, threads);
        }
    };
}

#endif //CRYPT_2_OFB_MODE_HPP
