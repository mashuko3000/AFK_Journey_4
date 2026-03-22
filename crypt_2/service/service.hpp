//
// Created by mvideo on 22/03/26.
//

#ifndef CRYPT_2_SERVICE_HPP
#define CRYPT_2_SERVICE_HPP

#include <string>
#include <fstream>
#include <filesystem>

#include "../inc/interfaces.hpp"
#include "../core_interfaces/crypto_enums.hpp"

class service final {
private:
    i_cipher& _cipher;

    void add_padding(bytes_t& data) const;
    void remove_padding(bytes_t& data) const;

public:
    explicit service(i_cipher& cipher) : _cipher(cipher) {}

    bytes_t encrypt_bytes(const bytes_t& data);
    bytes_t decrypt_bytes(const bytes_t& data);

    void encrypt_file(const std::filesystem::path& in, const std::filesystem::path& out);
    void decrypt_file(const std::filesystem::path& in, const std::filesystem::path& out);
};


#endif //CRYPT_2_SERVICE_HPP
