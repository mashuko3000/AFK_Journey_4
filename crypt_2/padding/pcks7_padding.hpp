//
// Created by mvideo on 06/03/26.
//

#ifndef CRYPT_2_PCKS7_PADDING_HPP
#define CRYPT_2_PCKS7_PADDING_HPP

#include"../core_interfaces/i_padding.hpp"

class PCKS7 : public IPadding
{
public:
    ~PCKS7() override = default;
    bytes_t add(bytes_t& data, size_t block_size) const override;
    bytes_t remove(bytes_t& data, size_t block_size) const override;
};
// PKCS7

#endif //CRYPT_2_PCKS7_PADDING_HPP
