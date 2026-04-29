//
// Created by maria on 07.04.2026.
//

#ifndef CRYPT_IPADDING_HPP
#define CRYPT_IPADDING_HPP

#include "../common/CryptoTypes.hpp"

namespace crypto
{
    class IPadding
    {
    public:
        virtual ~IPadding() = default;
        [[nodiscard]] virtual bytes_t add(const bytes_t& data, size_t block_size) const = 0;
        [[nodiscard]] virtual bytes_t remove(const bytes_t& data, size_t block_size) const = 0;
    };
}

#endif //CRYPT_IPADDING_HPP
