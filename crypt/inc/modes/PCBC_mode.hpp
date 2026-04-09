//
// Created by mvideo on 03/03/26.
//

#ifndef CRYPT_2_PCBC_MODE_HPP
#define CRYPT_2_PCBC_MODE_HPP

#include"../core/IMode.hpp"

namespace crypto
{
    class PCBC_mode : public IMode
{
    public:
        virtual ~PCBC_mode() override = default;

        bytes_t encrypt(const bytes_t& data,
                        const IBlockCipher* algo,
                        const bytes_t& iv, int threads) const override;

        bytes_t decrypt(const bytes_t& cipher,
                        const IBlockCipher* algo,
                        const bytes_t& iv, int threads) const override;

    private:
        void xor_blocks(bytes_t& target, const bytes_t& source) const;
    };
}


#endif //CRYPT_2_PCBC_MODE_HPP
