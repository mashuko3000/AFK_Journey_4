//
// Created by mvideo on 03/03/26.
//

#ifndef CRYPT_2_ANSI_PADDING_HPP
#define CRYPT_2_ANSI_PADDING_HPP

#include"../core_interfaces/i_padding.hpp"

class AnsiX923 : public IPadding
{
public:
    ~AnsiX923() noexcept override = default;
    bytes_t add(bytes_t& data, size_t block_size) const override;
    bytes_t remove(bytes_t& data, size_t block_size) const override;
};


#endif //CRYPT_2_ANSI_PADDING_HPP
