//
// Created by mvideo on 04/03/26.
//
#include"cipher_context.hpp"
#include <fstream>

CipherContext::CipherContext(
        i_cipher* alg,
        CipherMode& mode,
        PaddingType& padding,
        const bytes_t& iv,
        const AdditionalParams params)
        : _alg(alg), _iv(iv), _params(params), _mode(nullptr), _padding(nullptr)
{
    switch (mode)
    {
        case CipherMode::ECB:  _mode = new ECBMode(); break;
        case CipherMode::CBC:  _mode = new CBC_mode(); break;
        case CipherMode::PCBC: _mode = new PCBCMode(); break;

    }
    switch (padding)
    {
        case PaddingType::Zeros: _padding = new ZerosPadding(); break;
        case PaddingType::AnsiX923: _padding = new AnsiX923(); break;
        case PaddingType::PCKS7: _padding = new PCKS7(); break;
        case PaddingType::ISO10126: _padding = new ISO10126(); break;
    }
}

CipherContext::CipherContext(CipherContext&& other) noexcept
        : _alg(other._alg),
          _mode(other._mode),
          _padding(other._padding),
          _iv(std::move(other._iv)),
          _params(std::move(other._params))
{
    other._alg = nullptr;
    other._mode = nullptr;
    other._padding = nullptr;
}

CipherContext& CipherContext::operator=(CipherContext&& other) noexcept
{
    if (this != &other)
    {
        delete _mode;
        delete _padding;

        _alg = other._alg;
        _mode = other._mode;
        _padding = other._padding;
        _iv = std::move(other._iv);
        _params = std::move(other._params);

        other._alg = nullptr;
        other._mode = nullptr;
        other._padding = nullptr;
    }
    return *this;
}

void CipherContext::encrypt(
        const bytes_t& data,
        bytes_t& cipher,
        int threads
)
{
    bytes_t padded_data = _padding->add(const_cast<bytes_t&>(data), _alg->get_block_size());
    _mode->encrypt(padded_data, cipher, _alg, _iv, threads, _params);
}
void CipherContext::decrypt(
        bytes_t& cipher,
        bytes_t& data,
        int threads
)
{
    bytes_t decrypted_raw;
    _mode->decrypt(const_cast<bytes_t&>(cipher), decrypted_raw, _alg, _iv, threads, _params);
    data = _padding->remove(decrypted_raw, _alg->get_block_size());
}

std::future<void> CipherContext::encrypt_file(const std::string& in_path,
                               const std::string& out_path,
                               int threads)
{
    return std::async(std::launch::async, [this, in_path, out_path, threads]() {
        std::ifstream is(in_path, std::ios::binary);
        bytes_t data((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

        bytes_t result;
        this->encrypt(data, result, threads);

        std::ofstream os(out_path, std::ios::binary);
        os.write(reinterpret_cast<const char*>(result.data()), result.size());
    });
}
std::future<void> CipherContext::decrypt_file(const std::string& in_path,
                               const std::string& out_path,
                               int threads)
{
    return std::async(std::launch::async, [this, in_path, out_path, threads]() {
        std::ifstream is(in_path, std::ios::binary);
        bytes_t cipher((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

        bytes_t result;
        this->decrypt(cipher, result, threads);

        std::ofstream os(out_path, std::ios::binary);
        os.write(reinterpret_cast<const char*>(result.data()), result.size());
    });
}