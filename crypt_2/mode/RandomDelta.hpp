//
// Created by mvideo on 06/03/26.
//

#ifndef CRYPT_2_RANDOMDELTA_HPP
#define CRYPT_2_RANDOMDELTA_HPP

#include"CTR_mode.hpp"

class RandomDelta : public CTR_mode
{
protected:
    void next_counter(bytes_t& counter, const bytes_t& delta) const override;
    bytes_t get_counter_for_block(const bytes_t& iv, const bytes_t& delta, uint64_t block_idx) const override;
};

#endif //CRYPT_2_RANDOMDELTA_HPP
