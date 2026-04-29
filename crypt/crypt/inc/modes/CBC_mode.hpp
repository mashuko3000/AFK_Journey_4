//
// Created by mvideo on 03/03/26.
//

#ifndef CRYPT_2_CBC_MODE_HPP
#define CRYPT_2_CBC_MODE_HPP

#include"../core/IMode.hpp"
#include"../core/ICipher.hpp"

namespace crypto{
    class CBC_mode : public IMode
    {
        bytes_t encrypt(const bytes_t& data,
                        const IBlockCipher* algo,
                        const bytes_t& iv,
                        int threads) const override;

        bytes_t decrypt(const bytes_t& cipher,
                        const IBlockCipher* algo,
                        const bytes_t& iv,
                        int threads) const override;

    private:
        void xor_blocks(bytes_t& target, const bytes_t& source) const;
        void decrypt_range(const bytes_t& cipher, bytes_t& result, const IBlockCipher* algo, const bytes_t& iv, size_t start_block, size_t end_block) const;
    };
}


#endif //CRYPT_2_CBC_MODE_HPP
