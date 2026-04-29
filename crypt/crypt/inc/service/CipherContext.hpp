//
// Created by maria on 08.04.2026.
//

#ifndef CRYPT_CIPHERCONTEXT_HPP
#define CRYPT_CIPHERCONTEXT_HPP

#include "../core/ICipher.hpp"
#include "../core/IMode.hpp"
#include "../core/IPadding.hpp"
#include "../core/IBlockCipher.hpp"

#include<memory>
#include <future>

namespace crypto
{
    enum class CipherModeType { ECB, CBC, PCBC, CFB, OFB, CTR, RandomDelta };
    enum class PaddingModeType { Zeros, PKCS7, ISO10126, AnsiX923, NONE };

    class CipherContext final : public ICipher
    {
    private:
        std::shared_ptr<IBlockCipher> algo_;
        std::unique_ptr<IMode> mode_;
        std::unique_ptr<IPadding> padding_;
        bytes_t iv_;

    public:
        CipherContext(std::shared_ptr<IBlockCipher> algo,
                      CipherModeType mode_type,
                      PaddingModeType padding_type,
                      bytes_t iv = {},
                      int threads = 0);

        ~CipherContext() override = default;

        CipherContext(const CipherContext&) = delete;
        CipherContext& operator=(const CipherContext&) = delete;
        CipherContext(CipherContext&&) noexcept = default;
        CipherContext& operator=(CipherContext&&) noexcept = default;

        [[nodiscard]] std::string name() const override
        {
            return algo_->name() + "-Context";
        }

        [[nodiscard]] bool isSymmetric() const override
        {
            return true;
        }

        [[nodiscard]] bool isBlockCipher() const override
        {
            return false;
        }

        [[nodiscard]] bytes_t encrypt(const bytes_t& plaintext) override
        {
            return encrypt_multithreaded(plaintext, 1);
        }

        [[nodiscard]] bytes_t decrypt(const bytes_t& ciphertext) override
        {
            return decrypt_multithreaded(ciphertext, 1);
        }

        [[nodiscard]] bytes_t encrypt_multithreaded(const bytes_t& plaintext, int threads);
        [[nodiscard]] bytes_t decrypt_multithreaded(const bytes_t& ciphertext, int threads);

        [[nodiscard]] std::future<void> encrypt_file_async(const std::string& in_path,
                                                           const std::string& out_path,
                                                           int threads = 1);

        [[nodiscard]] std::future<void> decrypt_file_async(const std::string& in_path,
                                                           const std::string& out_path,
                                                           int threads = 1);
    };

} // namespace crypto

#endif //CRYPT_CIPHERCONTEXT_HPP
