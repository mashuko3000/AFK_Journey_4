//
// Created by maria on 18.04.2026.
//

#ifndef CRYPT_ASYMMETRICKEYS_HPP
#define CRYPT_ASYMMETRICKEYS_HPP

#include "../core/AsymmetricTraits.hpp"

namespace crypto
{
    class IKey
    {
    public:
        virtual ~IKey() = default;
        [[nodiscard]] virtual std::string algorithmName() const = 0;
    };

    template <typename Traits>
    class PublicKey final : public IKey
    {
    public:
        using Data = typename Traits::PublicKeyData;
    private:
        Data data_;
    public:
        explicit PublicKey(Data data) : data_(std::move(data)) {}
        [[nodiscard]] const Data& get_key() const noexcept {return data_;}
        [[nodiscard]] std::string algorithmName() const override {return Traits::ALGO_NAME;}
    };

    template <typename Traits>
    class PrivateKey final : public IKey
    {
    public:
        using Data = typename Traits::PrivateKeyData;
    private:
        Data data_;
        template <typename T> friend class AsymmetricBase;
        template <typename T> friend class SignatureBase;
    public:
        explicit PrivateKey(Data data) : data_(std::move(data)) {}
        [[nodiscard]] std::string algorithmName() const override {return Traits::ALGO_NAME;}
    };
}

#endif //CRYPT_ASYMMETRICKEYS_HPP
