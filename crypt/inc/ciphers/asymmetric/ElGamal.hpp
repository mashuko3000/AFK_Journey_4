//
// Created by maria on 18.04.2026.
//

#ifndef CRYPT_ELGAMAL_HPP
#define CRYPT_ELGAMAL_HPP

#include<memory>

#include "../../core/AsymmetricBase.hpp"
#include "../../core/AsymmetricTraits.hpp"
#include "../../core/IPrimalityTest.hpp"
#include "../../service/MillerRobinPrimalityTest.hpp"
#include <random>

namespace crypto
{
    class ElGamal final : public AsymmetricBase<ElGamalTraits>
    {
    private:
        std::unique_ptr<IPrimalityTest> primalityTest_;
        size_t bitLength = 128;
        double minProb = 0.9;

    public:
        ElGamal(std::unique_ptr<IPrimalityTest> test = nullptr,
                size_t bL = 2048,
                double mP = 0.999) : primalityTest_(std::move(test)), bitLength(bL), minProb(mP)
        {
            if (!primalityTest_)
            {
                primalityTest_ = std::make_unique<MillerRabinPrimalityTest>();
            }
            if (minProb < 0.5)
            {
                minProb = 0.5;
            }

            bool isPowerOfTwo = (bitLength > 0) && ((bitLength & (bitLength - 1)) == 0);
            if (bitLength < 128 || !isPowerOfTwo)
            {
                bitLength = 128;
            }
        }

        void generateKeyPair() override
        {
            BigInt p = generatePrime(bitLength);
            BigInt g = findGenerator(p);

            std::random_device rd;
            boost::random::mt19937 rng(rd());
            boost::random::uniform_int_distribution<BigInt> dist(2, p - 2);
            BigInt x = dist(rng);

            BigInt y = boost::multiprecision::powm(g, x, p);

            auto pubData = ElGamalTraits::PublicKeyData{p, g, y};
            auto privData = ElGamalTraits::PrivateKeyData{p, x};

            setupPublicKey(std::make_shared<PublicKey<ElGamalTraits>>(pubData));
            setupPrivateKey(std::make_shared<PrivateKey<ElGamalTraits>>(privData));
        }

    private:
        BigInt generatePrime(size_t bL)
        {
            std::random_device rd;
            boost::random::mt19937 rng(rd());
            while(true)
            {
                BigInt candidate = BigInt(1) << (bL - 1);
                boost::random::uniform_int_distribution<BigInt> dist(0, (BigInt(1) << (bL - 1)) - 1);
                candidate |= dist(rng);

                candidate|=1;

                if(primalityTest_->isPrime(candidate, minProb))
                {
                    return candidate;
                }
            }
        }

        BigInt findGenerator(const BigInt& p) const
        {
            BigInt q = (p - 1) / 2;
            for(BigInt g = 2; g < 100; ++g)
            {
                if (boost::multiprecision::powm(g, 2, p) != 1 &&
                    boost::multiprecision::powm(g, q, p) != 1) return g;
            }
            throw std::runtime_error("Primitive root not found (unlikely for safe primes)");
        }

        bytes_t encryptImpl(const PubData& pub, const bytes_t& data) const override
        {
            if(data.empty()) return {};

            BigInt m;
            boost::multiprecision::import_bits(m, data.begin(), data.end(), 8);

            if(m>=pub.p)
            {
                throw std::invalid_argument("Message is too large for this ElGamal key");
            }

            std::random_device rd;
            boost::random::mt19937 rng(rd());
            boost::random::uniform_int_distribution<BigInt> dist(2, pub.p - 2);
            BigInt k = dist(rng);

            BigInt c1 = boost::multiprecision::powm(pub.g, k, pub.p);
            BigInt s = boost::multiprecision::powm(pub.y, k, pub.p);
            BigInt c2 = (m * s) % pub.p;

            bytes_t b_c1, b_c2, result;
            boost::multiprecision::export_bits(c1, std::back_inserter(b_c1), 8);
            boost::multiprecision::export_bits(c2, std::back_inserter(b_c2), 8);

            uint32_t len = static_cast<uint32_t>(b_c1.size());
            result.push_back((len >> 24) & 0xFF);
            result.push_back((len >> 16) & 0xFF);
            result.push_back((len >> 8)  & 0xFF);
            result.push_back(len & 0xFF);

            result.insert(result.end(), b_c1.begin(), b_c1.end());
            result.insert(result.end(), b_c2.begin(), b_c2.end());

            return result;
        }

        bytes_t decryptImpl(const PrivData& priv, const bytes_t& data) const override
        {
            if(data.size() < 4)
            {
                throw std::runtime_error("Invalid ElGamal ciphertext");
            }

            uint32_t len1 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
            if(data.size() < 4 + len1) throw std::runtime_error("Corrupted ElGamal ciphertext");

            BigInt c1, c2;

            auto it = data.begin() + 4;

            boost::multiprecision::import_bits(c1, it, it + len1, 8);
            boost::multiprecision::import_bits(c2, it + len1, data.end(), 8);

            BigInt s = boost::multiprecision::powm(c1, priv.x, priv.p);
            BigInt sInv = boost::multiprecision::powm(s, priv.p - 2, priv.p);

            BigInt m = (c2 * sInv) % priv.p;

            bytes_t decrypted;
            boost::multiprecision::export_bits(m, std::back_inserter(decrypted), 8);
            return decrypted;
        }
    };
}

#endif //CRYPT_ELGAMAL_HPP
