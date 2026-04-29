//
// Created by maria on 18.04.2026.
//

#ifndef CRYPT_ASYMMETRICBASE_HPP
#define CRYPT_ASYMMETRICBASE_HPP

#include "IAsymmetricEncryptor.hpp"
#include "../keys/AsymmetricKeys.hpp"
#include <memory>
#include <stdexcept>

namespace crypto
{
    template <typename Traits>
    class AsymmetricBase : public IAsymmetricEncryptor
    {
    public:
        using PubData = typename Traits::PublicKeyData;
        using PrivData = typename Traits::PrivateKeyData;

        using SpecificPubKey = PublicKey<Traits>;
        using SpecificPrivKey = PrivateKey<Traits>;

    private:
        std::shared_ptr<SpecificPubKey> Pub_;
        std::shared_ptr<SpecificPrivKey> Priv_;

    public:
        [[nodiscard]] std::string name() const override {return Traits::ALGO_NAME;}

        void setupPublicKey(std::shared_ptr<SpecificPubKey> key)
        {
            if(!key) throw std::invalid_argument("Public key cannot be null");
            Pub_ = std::move(key);
        }

        void setupPrivateKey(std::shared_ptr<SpecificPrivKey> key)
        {
            if(!key) throw std::invalid_argument("Private key cannot be null");
            Priv_ = std::move(key);
        }

        [[nodiscard]] virtual bytes_t encrypt(const bytes_t& plaintext) override
        {
            if(!Pub_) throw std::runtime_error("Public key not set for " + name());
            return encryptImpl(Pub_->get_key(), plaintext);
        }

        [[nodiscard]] virtual bytes_t decrypt(const bytes_t& ciphertext) override
        {
            if(!Priv_) throw std::runtime_error("Private key not set for " + name());
            return decryptImpl(Priv_->data_, ciphertext);
        }

    protected:
        virtual bytes_t encryptImpl(const PubData& pub, const bytes_t& data) const = 0;
        virtual bytes_t decryptImpl(const PrivData& priv, const bytes_t& data) const = 0;
    };
}

#endif //CRYPT_ASYMMETRICBASE_HPP
