//
// Created by maria on 18.04.2026.
//

#ifndef CRYPT_SIGNATUREBASE_HPP
#define CRYPT_SIGNATUREBASE_HPP

#include "ISignatureScheme.hpp"
#include "../keys/AsymmetricKeys.hpp"

namespace crypto
{
    template<typename Traits>
    class SignatureBase : public ISignatureScheme
    {
    public:
        using PubData = typename Traits::PublicKeyData;
        using PrivData = typename Traits::PrivateKeyData;

        using SpecificPubKey = PublicKey<Traits>;
        using SpecificPrivKey = PrivateKey<Traits>;

    private:
        std::shared_ptr<SpecificPrivKey> Priv_;
        std::shared_ptr<SpecificPubKey> Pub_;

    public:
        [[nodiscard]] std::string name() const override {return Traits::ALGO_NAME;}
        [[nodiscard]] bytes_t sign(const bytes_t& message) override
        {
            if(!Priv_) throw std::runtime_error("Private key not set for signing");
            return signImpl(Priv_->data_, message);
        }
        [[nodiscard]] bool verify(const bytes_t& message, const bytes_t& signature) override
        {
            if(!Pub_) throw std::runtime_error("Public key not set for signing");
            return verifyImpl(Pub_.get_key(), message, signature);
        }

    public:
        void setPublicKey(std::shared_ptr<SpecificPubKey> key)
        {
            if (!key) throw std::invalid_argument("Public key cannot be null");
            Pub_ = std::move(key);
        }
        void setPrivateKey(std::shared_ptr<SpecificPrivKey> key)
        {
            if (!key) throw std::invalid_argument("Private key cannot be null");
            Priv_ = std::move(key);
        }

    protected:
        virtual bool verifyImpl(const PubData& pub, const bytes_t& message, const bytes_t& signature) const = 0;
        virtual bytes_t signImpl(const PrivData& priv, const bytes_t& message) const = 0;
    };
}

#endif //CRYPT_SIGNATUREBASE_HPP
