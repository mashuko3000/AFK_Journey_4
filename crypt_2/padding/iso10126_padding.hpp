//
// Created by mvideo on 06/03/26.
//

#ifndef CRYPT_2_ISO10126_PADDING_HPP
#define CRYPT_2_ISO10126_PADDING_HPP

#include"../core_interfaces/i_padding.hpp"
#include <random>

class ISO10126 final: public IPadding{
public:
    ISO10126() : gen(rd()){}
    ~ISO10126() override = default;
    bytes_t add(bytes_t& data, size_t block_size) const override;
    bytes_t remove(bytes_t& data, size_t block_size) const override;

private:
    mutable std::random_device rd;
    mutable std::mt19937 gen;
};


#endif //CRYPT_2_ISO10126_PADDING_HPP
