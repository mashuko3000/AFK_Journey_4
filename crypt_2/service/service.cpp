#include "service.hpp"
#include "../RSA/RSA.hpp"
#include <stdexcept>
#include <vector>
#include <algorithm>

namespace
{
    std::streamsize read_chunk(std::ifstream& is, char* buffer, size_t size)
    {
        is.read(buffer, size);
        return is.gcount();
    }
}

void service::add_padding(bytes_t& data) const
{
    size_t block_size = _cipher.get_block_size();
    size_t pad_len = block_size - (data.size() % block_size);

    for (size_t i = 0; i < pad_len; ++i)
    {
        data.push_back(static_cast<byte_t>(pad_len));
    }
}

void service::remove_padding(bytes_t& data) const
{
    if (data.empty()) return;

    byte_t pad_len = data.back();
    size_t block_size = _cipher.get_block_size();

    if (pad_len > 0 && pad_len <= block_size && pad_len <= data.size())
    {
        bool valid_padding = true;
        for (size_t i = data.size() - pad_len; i < data.size(); ++i)
        {
            if (data[i] != pad_len)
            {
                valid_padding = false;
                break;
            }
        }

        if (valid_padding)
        {
            data.resize(data.size() - pad_len);
        }
    }
}

bytes_t service::encrypt_bytes(const bytes_t& data)
{
    size_t in_block_size = _cipher.get_block_size();
    bytes_t result;
    bytes_t temp_data = data;

    add_padding(temp_data);

    for (size_t i = 0; i < temp_data.size(); i += in_block_size)
    {
        bytes_t block(temp_data.begin() + i, temp_data.begin() + i + in_block_size);
        bytes_t enc = _cipher.encrypt_block(block);
        result.insert(result.end(), enc.begin(), enc.end());
    }
    return result;
}

bytes_t service::decrypt_bytes(const bytes_t& data)
{
    size_t step;
    if (auto* rsa = dynamic_cast<RSACipher*>(&_cipher))
    {
        step = rsa->get_full_modulus_size();
    }
    else
    {
        step = _cipher.get_block_size();
    }

    bytes_t result;

    for (size_t i = 0; i < data.size(); i += step)
    {
        size_t current_chunk = std::min(step, data.size() - i);
        bytes_t block(data.begin() + i, data.begin() + i + current_chunk);

        bytes_t dec = _cipher.decrypt_block(block);
        result.insert(result.end(), dec.begin(), dec.end());
    }

    remove_padding(result);
    return result;
}

void service::encrypt_file(const std::filesystem::path& in, const std::filesystem::path& out)
{
    std::ifstream is(in, std::ios::binary);
    std::ofstream os(out, std::ios::binary);
    if (!is || !os) throw std::runtime_error("Cannot open file(s) for encryption.");

    size_t in_block_size = _cipher.get_block_size();
    std::vector<char> buffer(in_block_size);

    while (is.read(buffer.data(), in_block_size))
    {
        bytes_t block(buffer.begin(), buffer.end());
        bytes_t enc = _cipher.encrypt_block(block);
        os.write(reinterpret_cast<const char*>(enc.data()), enc.size());
    }

    size_t read_bytes = is.gcount();
    bytes_t last_block(buffer.begin(), buffer.begin() + read_bytes);

    add_padding(last_block);
    bytes_t enc = _cipher.encrypt_block(last_block);
    os.write(reinterpret_cast<const char*>(enc.data()), enc.size());
}

void service::decrypt_file(const std::filesystem::path& in, const std::filesystem::path& out)
{
    std::ifstream is(in, std::ios::binary);
    std::ofstream os(out, std::ios::binary);
    if (!is || !os) return;

    size_t step;
    if (auto* rsa = dynamic_cast<RSACipher*>(&_cipher))
    {
        step = rsa->get_full_modulus_size();
    }
    else
    {
        step = _cipher.get_block_size();
    }

    std::vector<char> buffer(step);
    bytes_t last_block;
    bool first = true;

    while (is.read(buffer.data(), step) || is.gcount() > 0)
    {
        bytes_t block(buffer.begin(), buffer.begin() + is.gcount());
        if (block.empty()) break;

        bytes_t dec = _cipher.decrypt_block(block);

        if (!first)
        {
            os.write(reinterpret_cast<const char*>(last_block.data()), last_block.size());
        }
        last_block = dec;
        first = false;
    }

    if (!first)
    {
        remove_padding(last_block);
        os.write(reinterpret_cast<const char*>(last_block.data()), last_block.size());
    }
}