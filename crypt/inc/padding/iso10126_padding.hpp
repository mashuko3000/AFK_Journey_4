//
// Created by mvideo on 06/03/26.
//

#ifndef CRYPT_2_ISO10126_PADDING_HPP
#define CRYPT_2_ISO10126_PADDING_HPP

#include "../core/IPadding.hpp"
#include <random>

namespace crypto
{
    class ISO10126 final : public IPadding
    {
    public:
        ISO10126() : gen(rd()) {}
        ~ISO10126() override = default;

        [[nodiscard]] bytes_t add(const bytes_t& data, size_t block_size) const override;
        [[nodiscard]] bytes_t remove(const bytes_t& data, size_t block_size) const override;

    private:
        mutable std::random_device rd;
        mutable std::mt19937 gen;
    };
}


#endif //CRYPT_2_ISO10126_PADDING_HPP
