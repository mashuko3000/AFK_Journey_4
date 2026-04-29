//
// Created by mvideo on 03/03/26.
//

#ifndef CRYPT_2_ECB_MODE_HPP
#define CRYPT_2_ECB_MODE_HPP

#include"../core/IMode.hpp"

#include <algorithm>
#include <future>
#include <vector>

namespace crypto
{
    class ECB_mode : public IMode
{
    public:
        bytes_t encrypt(const bytes_t& data,
                        const IBlockCipher* algo,
                        const bytes_t& iv,
                        int threads) const override;

        bytes_t decrypt(const bytes_t& cipher,
                        const IBlockCipher* algo,
                        const bytes_t& iv,
                        int threads) const override;

    private:
        void process_blocks(const bytes_t& input,
                            bytes_t& output,
                            const IBlockCipher* algo,
                            size_t start_block,
                            size_t end_block,
                            bool encrypt) const;
    };
}



#endif //CRYPT_2_ECB_MODE_HPP
