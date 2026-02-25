#ifndef CRYPT_2_UTILS_HPP
#define CRYPT_2_UTILS_HPP

#include "des.hpp"
#include <string>
#include <fstream>
#include <filesystem>

class service final
{
private:
    i_cipher& _cipher;

    void add_padding(bytes_t& data) const;
    void remove_padding(bytes_t& data) const;

public:
    explicit service(i_cipher& cipher) : _cipher(cipher) {};

    bytes_t encrypt_bytes(const bytes_t& data);
    bytes_t decrypt_bytes(const bytes_t& data);

    void encrypt_file(const std::filesystem::path& input_path, const std::filesystem::path& output_path);
    void decrypt_file(const std::filesystem::path& input_path, const std::filesystem::path& output_path);
};

#endif //CRYPT_2_UTILS_HPP
