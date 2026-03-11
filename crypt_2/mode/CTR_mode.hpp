//
// Created by mvideo on 06/03/26.
//

#ifndef CRYPT_2_CTR_MODE_HPP
#define CRYPT_2_CTR_MODE_HPP

#include"../core_interfaces/i_mode.hpp"

class CTR_mode : public IMode
{
public:
    virtual ~CTR_mode() override = default;
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
protected:
    virtual void process_range(
            const bytes_t& input,
            bytes_t& output,
            i_cipher* algo,
            const bytes_t& iv,
            size_t start_pos,
            size_t end_pos
    ) const;
    bytes_t prepare_counter_block(const bytes_t& iv, uint64_t offset) const;
    virtual void next_counter(bytes_t& counter, const bytes_t& delta) const;
    virtual bytes_t get_counter_for_block(const bytes_t& iv, const bytes_t& delta, uint64_t block_idx) const;
    void increment_block(bytes_t& block) const;
};


#endif //CRYPT_2_CTR_MODE_HPP
