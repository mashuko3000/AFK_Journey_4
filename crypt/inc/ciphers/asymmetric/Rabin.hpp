//
// Created by maria on 05.05.2026.
//

#ifndef CRYPT_RABIN_HPP
#define CRYPT_RABIN_HPP

#include "../../core/AsymmetricBase.hpp"
#include "../../core/AsymmetricTraits.hpp"
#include "../../math/NumberTheoryService.hpp"
#include "../../service/MillerRobinPrimalityTest.hpp"
#include "core/IPrimalityTest.hpp"

#include <boost/random.hpp>
#include <vector>
#include <memory>
#include <unordered_set>

namespace crypto
{
    class RabinEncryptor : public AsymmetricBase<RabinTraits>
    {
    private:
        size_t blockSize_;

        std::vector<bigint> getSmallPrimes()
        {
            static std::vector<bigint> p = getFirstNPrimes(500);
            return p;
        }

        bigint generateBlumPrime(size_t bits)
        {
            static boost::random::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
            MillerRabinPrimalityTest primalityTest;

            const auto& smallPrimes = getSmallPrimes();

            bigint min = (bigint(1) << (bits - 1));
            bigint max = (bigint(1) << bits) - 1;
            boost::random::uniform_int_distribution<bigint> dist(min, max);

            while (true)
            {
                bigint p = dist(gen);

                p |= 3;
                p |= min;

                bool isComposite = false;

                for (const bigint& prime : smallPrimes)
                {
                    if (p & prime == 0)
                    {
                        isComposite = true;
                        break;
                    }
                }

                if(isComposite) continue;

                if (primalityTest.isPrime(p, 0.9999))
                {
                    return p;
                }
            }
        }

        std::vector<bigint> getFirstNPrimes(int count)
        {
            std::vector<bigint> primes;
            if(count <= 0) return primes;

            primes.push_back(2);
            //std::cout << 2 << ", ";
            bigint start = 3;
            int step = 2;

            MillerRabinPrimalityTest ptest;
            int k = 2;

            while(k < count)
            {
                if(ptest.isPrime(start, 0.9999))
                {
                    primes.push_back(start);
                    k += 1;
                    /*
                    if(k != (count - 1))
                    {
                        std::cout << start << ", ";
                        k += 1;
                    }
                    else
                    {
                        std::cout << start << std::endl;
                        break;
                    }*/
                }
                start += 2;
                step = 6 - step;
            }
            return primes;
        }
        
        std::vector<bigint> getRandomPrimes(int count)
        {
            std::vector<bigint> result;
            if (count <= 0) return result;
            int k = 0;
            static boost::random::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
            MillerRabinPrimalityTest primalityTest;

            bigint max = getRoughUpperLimit(count);
            bigint min = 3;
            
            boost::random::uniform_int_distribution<bigint> dist(min, max);
            std::unordered_set<bigint> primes;

            while(k < count)
            {
                bigint p = dist(gen);
                if(!(p & 1)) continue;

                if(primes.find(p) != primes.end()) continue;

                if(primalityTest.isPrime(p, 0.9999))
                {
                    primes.insert(p);
                    k += 1;
                    result.push_back(p);
                    //std::cout << p << ", ";
                }
            }
            return result;
        }

        //Rough estimate of the upper bound by using a binary
        // log to calculate -> log_2(x) or the position of the MSB
        bigint getRoughUpperLimit(int count)
        {
            if(count < 2) return 100;
            int bits = 0;
            int temp = count;

            while(temp > 0)
            {
                bits++;
                temp >>=1;
            }

            return bigint(count) * bits * 4;
        }

    public:
        RabinEncryptor(size_t keySizeBits = 1024) : blockSize_((keySizeBits / 8) - 6) {}

        void generateKeyPair() override
        {
            bigint p = generateBlumPrime(512);
            bigint q = generateBlumPrime(512);

            while (p == q) q = generateBlumPrime(512);

            PubData pubData;
            pubData.n = p * q;

            auto pubKey = std::make_shared<SpecificPubKey>(pubData);
            setupPublicKey(pubKey);

            PrivData privData;
            privData.p = p;
            privData.q = q;

            auto privKey = std::make_shared<SpecificPrivKey>(privData);
            setupPrivateKey(privKey);
        }

    protected:
        bytes_t encryptImpl(const PubData &pub, const bytes_t &data) const override
        {
            bytes_t ciphertext;
            size_t encBlockSize = pub.n == 0 ? 0 : (msb(pub.n) / 8 + 1);

            for (size_t i = 0; i < data.size(); i += blockSize_)
            {
                bytes_t chunk(data.begin() + i, data.begin() + std::min(data.size(), i + blockSize_));

                bytes_t paddedChunk;

                paddedChunk.push_back(0x01);
                paddedChunk.insert(paddedChunk.end(), chunk.begin(), chunk.end());

                paddedChunk.push_back(0xAB);
                paddedChunk.push_back(0xCD);
                paddedChunk.push_back(0xEF);
                paddedChunk.push_back(0xCE);

                // c = m^2 mod(n)
                bigint m = bytesToBigint(paddedChunk);
                bigint c = NumberTheoryService::modPow(m, 2, pub.n);

                bytes_t encryptedChunk = bigintToBytes(c, encBlockSize);
                ciphertext.insert(ciphertext.end(), encryptedChunk.begin(), encryptedChunk.end());
            }
            return ciphertext;
        }

        bytes_t decryptImpl(const PrivData &priv, const bytes_t &data) const override
        {
            bytes_t plaintext;
            bigint n = priv.p * priv.q;

            size_t encBlockSize = n == 0 ? 0 : (msb(n) / 8 + 1);

            if (encBlockSize == 0 || data.size() % encBlockSize != 0)
            {
                throw std::runtime_error("Invalid ciphertext size");
            }

            for (size_t i = 0; i < data.size(); i += encBlockSize)
            {
                bytes_t chunk(data.begin() + i, data.begin() + i + encBlockSize);
                bigint c = bytesToBigint(chunk);

                bigint m_p = NumberTheoryService::modPow(c, (priv.p + 1) / 4, priv.p);
                bigint m_q = NumberTheoryService::modPow(c, (priv.q + 1) / 4, priv.q);

                bigint yp, yq;
                NumberTheoryService::extended_gcd(priv.p, priv.q, yp, yq);

                bigint r1 = (m_p * priv.q * yq + m_q * priv.p * yp) % n;
                bigint r2 = (m_p * priv.q * yq - m_q * priv.p * yp) % n;

                if (r1 < 0) r1 += n;
                if (r2 < 0) r2 += n;

                std::vector<bigint> roots = {
                        r1,
                        n - r1,
                        r2,
                        n - r2
                };

                bool found = false;
                for (const auto &r : roots)
                {
                    bytes_t candidate = bigintToBytes(r);

                    if (candidate.size() >= 5)
                    {
                        size_t sz = candidate.size();
                        if (candidate[sz - 4] == 0xAB && candidate[sz - 3] == 0xCD &&
                            candidate[sz - 2] == 0xEF && candidate[sz - 1] == 0xCE) {

                            size_t startOffset = (candidate[0] == 0x01) ? 1 : 0;

                            plaintext.insert(plaintext.end(),
                                            candidate.begin() + startOffset,
                                            candidate.end() - 4);
                            found = true;
                            break;
                        }
                    }
                }
                if (!found) throw std::runtime_error("Rabin Decryption Error: Redundancy check failed.");
            }
            return plaintext;
        }

    private:
        static bigint bytesToBigint(const bytes_t &b)
        {
            bigint res = 0;
            for (uint8_t byte: b)
            {
                res = (res << 8) | byte;
            }
            return res;
        }

        static bytes_t bigintToBytes(bigint val, size_t fixedSize = 0)
        {
            bytes_t res;
            if (val == 0 && fixedSize == 0)
            {
                return {0x00};
            }
            while (val > 0)
            {
                res.insert(res.begin(), static_cast<uint8_t>(val & 0xFF));
                val >>= 8;
            }
            while (fixedSize > 0 && res.size() < fixedSize)
            {
                res.insert(res.begin(), 0x00);
            }
            return res;
        }
    };
}

#endif //CRYPT_RABIN_HPP
