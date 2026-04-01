#include "utils.hpp"

void service::add_padding(bytes_t& data) const
{
    size_t pad_len = 8 - (data.size() % 8);
    for (size_t i = 0; i < pad_len; ++i)
    {
        data.push_back(static_cast<byte_t>(pad_len));
    }
}

void service::remove_padding(bytes_t& data) const
{
    if (data.empty()) return;
    byte_t pad_len = data.back();
    if (pad_len > 0 && pad_len <= 8)
    {
        data.resize(data.size() - pad_len);
    }
}

bytes_t service::encrypt_bytes(const bytes_t& data)
{
    bytes_t padded_data = data;
    add_padding(padded_data);

    bytes_t result;
    for (size_t i = 0; i < padded_data.size(); i += 8)
    {
        bytes_t block(padded_data.begin() + i, padded_data.begin() + i + 8);
        bytes_t enc = _cipher.encrypt_block(block);
        result.insert(result.end(), enc.begin(), enc.end());
    }
    return result;
}

bytes_t service::decrypt_bytes(const bytes_t& data)
{
    bytes_t result;
    for (size_t i = 0; i < data.size(); i += 8)
    {
        bytes_t block(data.begin() + i, data.begin() + i + 8);
        bytes_t dec = _cipher.decrypt_block(block);
        result.insert(result.end(), dec.begin(), dec.end());
    }
    remove_padding(result);
    return result;
}

void service::encrypt_file(const std::filesystem::path& in, const std::filesystem::path& out)
{
    std::ifstream is(in, std::ios::binary);
    bytes_t buffer((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

    bytes_t encrypted = encrypt_bytes(buffer);

    std::ofstream os(out, std::ios::binary);
    os.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
}

void service::decrypt_file(const std::filesystem::path& in, const std::filesystem::path& out)
{
    std::ifstream is(in, std::ios::binary);
    bytes_t buffer((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

    bytes_t decrypted = decrypt_bytes(buffer);

    std::ofstream os(out, std::ios::binary);
    os.write(reinterpret_cast<const char*>(decrypted.data()), decrypted.size());
}
