//
// Created by mvideo on 03/03/26.
//

#ifndef CRYPT_2_ANSI_PADDING_HPP
#define CRYPT_2_ANSI_PADDING_HPP

#include "../core/IPadding.hpp"

namespace crypto
{
    class AnsiX923 final : public IPadding
    {
    public:
        ~AnsiX923() override = default;

        [[nodiscard]] bytes_t add(const bytes_t& data, size_t block_size) const override;
        [[nodiscard]] bytes_t remove(const bytes_t& data, size_t block_size) const override;
    };
}


#endif //CRYPT_2_ANSI_PADDING_HPP
