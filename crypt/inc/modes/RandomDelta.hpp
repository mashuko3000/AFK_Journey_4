//
// Created by mvideo on 06/03/26.
//

#ifndef CRYPT_2_RANDOMDELTA_HPP
#define CRYPT_2_RANDOMDELTA_HPP

#include"CTR_mode.hpp"

namespace crypto
{
    class RandomDelta : public CTR_mode
{
    public:
        explicit RandomDelta(const bytes_t& delta);
        explicit RandomDelta(size_t block_size);

    protected:
        void next_counter(bytes_t& counter) const override;
        bytes_t get_counter_for_block(const bytes_t& iv, uint64_t block_idx) const override;

    private:
        bytes_t delta_;
    };
}

#endif //CRYPT_2_RANDOMDELTA_HPP
