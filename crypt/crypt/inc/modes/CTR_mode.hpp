//
// Created by mvideo on 06/03/26.
//

#ifndef CRYPT_2_CTR_MODE_HPP
#define CRYPT_2_CTR_MODE_HPP

#include"../core/IMode.hpp"

namespace crypto
{
    class CTR_mode : public IMode {
    public:
        virtual ~CTR_mode() override = default;

        bytes_t encrypt(const bytes_t& data,
                        const IBlockCipher* algo,
                        const bytes_t& iv,
                        int threads) const override;

        bytes_t decrypt(const bytes_t& cipher,
                        const IBlockCipher* algo,
                        const bytes_t& iv,
                        int threads) const override;

    protected:
        virtual void process_range(const bytes_t& input,
                                   bytes_t& output,
                                   const IBlockCipher* algo,
                                   const bytes_t& iv,
                                   size_t start_pos,
                                   size_t end_pos) const;

        virtual void next_counter(bytes_t& counter) const;

        virtual bytes_t get_counter_for_block(const bytes_t& iv,
                                              uint64_t block_idx) const;
    };
}


#endif //CRYPT_2_CTR_MODE_HPP
