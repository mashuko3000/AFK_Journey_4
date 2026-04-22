#include"../../inc/service/CipherContext.hpp"

#include "../../inc/modes/ECB_mode.hpp"
#include "../../inc/modes/CBC_mode.hpp"
#include "../../inc/modes/CFB_mode.hpp"
#include "../../inc/modes/OFB_mode.hpp"
#include "../../inc/modes/CTR_mode.hpp"
#include "../../inc/modes/PCBC_mode.hpp"
#include "../../inc/modes/RandomDelta.hpp"

#include "../../inc/padding/ansi_padding.hpp"
#include "../../inc/padding/pkcs7_padding.hpp"
#include "../../inc/padding/zeros_padding.hpp"
#include "../../inc/padding/iso10126_padding.hpp"

#include <fstream>

namespace crypto {
    std::unique_ptr<IMode> createMode(CipherModeType mode_type)
    {
        switch (mode_type)
        {
            case CipherModeType::ECB : return std::make_unique<ECB_mode>();
            case CipherModeType::CBC : return std::make_unique<CBC_mode>();
            case CipherModeType::PCBC : return std::make_unique<PCBC_mode>();
            case CipherModeType::CFB : return std::make_unique<CFB_mode>();
            case CipherModeType::OFB : return std::make_unique<OFB_mode>();
            case CipherModeType::CTR : return std::make_unique<CTR_mode>();
            default : throw std::invalid_argument("CipherContext: Unsupported cipher mode");
        }
    }

    std::unique_ptr<IPadding> createPadding(PaddingModeType type)
    {
        switch (type)
        {
            case PaddingModeType::PKCS7 : return std::make_unique<PKCS7>();
            case PaddingModeType::Zeros : return std::make_unique<ZerosPadding>();
            case PaddingModeType::ISO10126 : return std::make_unique<ISO10126>();
            case PaddingModeType::AnsiX923 : return std::make_unique<AnsiX923>();
            case PaddingModeType::NONE : return nullptr;
            default : throw std::invalid_argument("CipherContext: Unsupported padding type");
        }
    }

    CipherContext::CipherContext(std::shared_ptr<IBlockCipher> algo,
                                 CipherModeType mode_type,
                                 PaddingModeType padding_type,
                                 bytes_t iv,
                                 int threads)
                                 :
                                 algo_(algo),
                                 padding_(createPadding(padding_type)),
                                 iv_(std::move(iv))
                                 {
        if (!algo_)
        {
            throw std::invalid_argument("CipherContext: block cipher algorithm cannot be null");
        }

        if(mode_type == CipherModeType::RandomDelta)
        {
            mode_ = std::make_unique<RandomDelta>(algo_->getBlockSize());
        }
        else
        {
            mode_ = createMode(mode_type);
        }



        if (!iv_.empty() && iv_.size() != algo_->getBlockSize())
        {
            throw std::invalid_argument("[CipherContext] IV size must exactly match the block size of the algorithm");
        }
    }

    bytes_t CipherContext::encrypt_multithreaded(const bytes_t& plaintext, int threads)
    {
        if(threads <= 0)
        {
            throw std::runtime_error("Count of threads should be more than 0");
        }

        if(!mode_)
        {
            throw std::logic_error("[CipherContext] Mode is not initialized");
        }

        bytes_t data_to_encrypt = plaintext;

        if(padding_)
        {
            data_to_encrypt = padding_->add(data_to_encrypt, algo_->getBlockSize());
        }

        return mode_->encrypt(data_to_encrypt, algo_.get(), iv_, threads);
    }

    bytes_t CipherContext::decrypt_multithreaded(const bytes_t& ciphertext, int threads)
    {
        if(threads <= 0)
        {
            throw std::runtime_error("Count of threads should be more than 0");
        }

        if(!mode_)
        {
            throw std::logic_error("[CipherContext] Mode is not initialized");
        }

        bytes_t plaintext = mode_->decrypt(ciphertext, algo_.get(), iv_, threads);

        return padding_
        ? padding_ ->remove(plaintext, algo_->getBlockSize())
        : plaintext
        ;
    }

    std::future<void> CipherContext::encrypt_file_async(const std::string& in_path,
                                                       const std::string& out_path,
                                                       int threads)
    {
        return std::async(std::launch::async,
                          [this, in_path, out_path, threads]()
                          {
            std::ifstream is(in_path, std::ios::binary);
            std::ofstream os(out_path, std::ios::binary);

            if(!is || !os)
            {
                throw std::runtime_error("CipherContext::encrypt_file_async: failed to open files");
            }

            const size_t block_size = algo_->getBlockSize();
            const size_t chunk_size = ((1024ULL * 1024) / block_size) * block_size;

            bytes_t buffer(chunk_size);
            bytes_t current_iv = iv_;
            bytes_t pt;

            while(true)
            {
                is.read(reinterpret_cast<char*>(buffer.data()), chunk_size);
                const size_t bytes_read = is.gcount();

                if(bytes_read == 0) break;

                if(!pt.empty())
                {
                    bytes_t encrypted = mode_->encrypt(pt,
                                                       algo_.get(),
                                                       current_iv,
                                                       threads);

                    os.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());

                    if (encrypted.size() >= block_size)
                    {
                        current_iv.assign(encrypted.end() - block_size, encrypted.end());
                    }
                }
                pt.assign(buffer.begin(), buffer.begin() + bytes_read);
            }

            if(!pt.empty())
            {
                bytes_t to_encrypt = std::move(pt);
                if (padding_)
                {
                    to_encrypt = padding_->add(to_encrypt, block_size);
                }

                bytes_t encrypted = mode_->encrypt(to_encrypt, algo_.get(), current_iv, threads);
                os.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
            }
                          });
    }

    std::future<void> CipherContext::decrypt_file_async(const std::string& in_path,
                                                       const std::string& out_path,
                                                       int threads)
    {
        return std::async(std::launch::async,
                          [this, in_path, out_path, threads]
        {
            std::ifstream is(in_path, std::ios::binary);
            std::ofstream os(out_path, std::ios::binary);

            if(!is || !os)
            {
                throw std::runtime_error("CipherContext::encrypt_file_async: failed to open files");
            }

            const size_t block_size = algo_->getBlockSize();
            const size_t chunk_size = ((1024ULL * 1024) / block_size) * block_size;

            bytes_t buffer(chunk_size);
            bytes_t current_iv = iv_;
            bytes_t last_decrypted;

            while(
                    is.read(reinterpret_cast<char*>(buffer.data()), chunk_size) ||
                    is.gcount() > 0
                    )
            {
                const size_t bytes_read = is.gcount();

                if(bytes_read < chunk_size)
                {
                    buffer.resize(bytes_read);
                }

                bytes_t next_iv;
                if(bytes_read >= block_size)
                {
                    next_iv.assign(buffer.end() - block_size, buffer.end());
                }

                bytes_t decrypted = mode_->decrypt(buffer, algo_.get(), current_iv, threads);

                if(!last_decrypted.empty())
                {
                    os.write(reinterpret_cast<const char*>(last_decrypted.data()), last_decrypted.size());
                }

                last_decrypted = std::move(decrypted);
                current_iv = std::move(next_iv);
            }

            if(!last_decrypted.empty())
            {
                bytes_t final_plaintext;
                if (padding_)
                {
                    final_plaintext = padding_->remove(last_decrypted, block_size);
                }
                else
                {
                    final_plaintext = std::move(last_decrypted);
                }
                os.write(reinterpret_cast<const char*>(final_plaintext.data()), final_plaintext.size());
            }
        });
    }
}

