
#ifndef CRYPT_2_CIPHER_CONTEXT_HPP
#define CRYPT_2_CIPHER_CONTEXT_HPP

#include"interfaces.hpp"
#include "crypro_types.hpp"
#include"../core_interfaces/crypto_enums.hpp"
#include"../core_interfaces/i_mode.hpp"
#include "../core_interfaces/i_padding.hpp"
#include <future>
#include<chrono>
#include"../mode/CBC_mode.hpp"
#include"../mode/ECB_mode.hpp"
#include"../mode/PCBC_mode.hpp"
#include "../padding/zeros_padding.hpp"
#include "../padding/ansi_padding.hpp"

class CipherContext
{
private:
    i_cipher* _alg;
    IMode* _mode;
    IPadding* _padding;
    bytes_t _iv;
    AdditionalParams _params;
public:
    ~CipherContext()
    {
        delete _mode;
        delete _padding;
    }

    CipherContext(
            i_cipher* alg,
            CipherMode& mode,
            PaddingType& padding,
            const bytes_t& iv = {},
            const AdditionalParams params = {});

    CipherContext(const CipherContext& other) = delete;
    CipherContext& operator=(CipherContext& other) = delete;
    CipherContext(CipherContext&& other) noexcept;
    CipherContext& operator=(CipherContext&& other) noexcept;

    void encrypt(
            const bytes_t& data,
            bytes_t& cipher,
            int threads = 1
            );
    void decrypt(
            bytes_t& cipher,
            bytes_t& data,
            int threads = 1
            );

    std::future<void> encrypt_file(const std::string& in_path,
                                   const std::string& out_path,
                                   int threads = 1);
    std::future<void> decrypt_file(const std::string& in_path,
                                   const std::string& out_path,
                                   int threads = 1);
};

#endif //CRYPT_2_CIPHER_CONTEXT_HPP
