//
// Created by maria on 22.04.2026.
//

#ifndef CRYPT_NTRU_HPP
#define CRYPT_NTRU_HPP

#include "../../core/AsymmetricBase.hpp"
#include "../../math/NtruPolynomial.hpp"

namespace crypto
{
    class NTRU final : public AsymmetricBase<NTRUTraits>
    {
    private:
        int N_;
        ll p_;
        ll q_;
        int df_;
        int dg_;

    public:
        explicit NTRU(int N, ll p, ll q, int df, int dg)
                : N_(N), p_(p), q_(q), df_(df), dg_(dg)
        {
            if(N <= 0 || p <= 1 || q <= 1) throw std::invalid_argument("N, p, q must be positive and > 1");
            if(2 * df_ - 1 > N) throw std::invalid_argument("df is too large for the given N");
            if(2 * dg_ > N) throw std::invalid_argument("dg is too large for the given N");
            if (q <= p || (q & (q-1)) != 0) throw std::invalid_argument("q must be a power of 2 and > p");
            if (std::gcd(p, q) != 1) throw std::invalid_argument("p and q must be coprime (gcd(p, q) == 1)"); //
        }

        void generateKeyPair() override
        {
            NtruPolynomial f(N_, q_);
            NtruPolynomial f_q_inv(N_, q_);
            NtruPolynomial f_p_inv(N_, p_);

            while(true)
            {
                f = NtruPolynomial::generateTernary(N_, q_, df_, df_ - 1);
                try{
                    std::cout << "Step..." << std::endl;
                    f_q_inv = f.invertModuloPowerOfTwo();
                    NtruPolynomial f_mod_p = f.changeMod(p_);
                    f_p_inv = f_mod_p.invertModPrime(p_);
                    break;
                } catch (const std::runtime_error&)
                {
                    continue;
                }
            }

            NtruPolynomial g = NtruPolynomial::generateTernary(N_, q_, dg_, dg_);
            NtruPolynomial h = (f_q_inv * g) * p_;

            NTRUTraits::PublicKeyData PubData{h};
            NTRUTraits::PrivateKeyData PrivData{f, f_p_inv};
            auto pubKey = std::make_shared<SpecificPubKey>(PubData);
            auto privKey = std::make_shared<SpecificPrivKey>(PrivData);

            setupPublicKey(pubKey);
            setupPrivateKey(privKey);
        }

    private:
        NtruPolynomial encodeBToP(const bytes_t& data) const
        {
            std::vector<ll> coeffs(N_, 0);
            int bitIDX = 0;
            uint8_t size = data.size();

            for(int j = 0; j < 8; ++j)
            {
                coeffs[bitIDX++] = (size >> j) & 1;
            }

            for(uint8_t byte : data)
            {
                for(int j = 0; j < 8; ++j)
                {
                    coeffs[bitIDX++] = (byte >> j) & 1;
                }
            }

            return NtruPolynomial(coeffs, N_, p_);
        }

        bytes_t decodePolyToBytes(const NtruPolynomial& poly) const
        {
            bytes_t result;
            int bit_idx = 0;

            uint8_t size = 0;
            for (int j = 0; j < 8; ++j)
            {
                if (poly[bit_idx++] == 1)
                {
                    size |= (1 << j);
                }
            }

            for (int i = 0; i < size; ++i)
            {
                uint8_t byte = 0;
                for (int j = 0; j < 8; ++j)
                {
                    long long val = poly[bit_idx++];
                    if (val < 0) val += p_;
                    if (val == 1)
                    {
                        byte |= (1 << j);
                    }
                }
                result.push_back(byte);
            }

            return result;
        }

        bytes_t encryptImpl(const PubData& pub, const bytes_t& data) const override
        {
            NtruPolynomial m = encodeBToP(data);
            NtruPolynomial r = NtruPolynomial::generateTernary(N_, q_, dg_, dg_);

            NtruPolynomial h = pub.h;

            h = h.changeMod(q_);

            NtruPolynomial c = (r * h) + m.changeMod(q_);

            return packCoefficients(c);
        }


        bytes_t decryptImpl(const PrivData& priv, const bytes_t& data) const override
        {
            NtruPolynomial c = unpackCoefficients(data);
            NtruPolynomial f = priv.f;
            f.changeMod(q_);
            NtruPolynomial a = f * c;
            a.center();

            NtruPolynomial fp = priv.fp;
            fp.changeMod(p_);
            NtruPolynomial m = fp * a.changeMod(p_);

            return decodePolyToBytes(m);
        }

        [[nodiscard]] NtruPolynomial mul(const NtruPolynomial& a, const NtruPolynomial& b) const
        {
            std::vector<ll> res = multiply_cyclic(a.getCoeffs(), b.getCoeffs());
            NtruPolynomial result(N_, q_);
            for (size_t i = 0; i < res.size(); ++i)
                result[i % N_] = (result[i % N_] + res[i]) % q_;
            return result;
        }

        bytes_t packCoefficients(const NtruPolynomial& poly) const
        {
            int bitsPerCoeff = getBitsPerCoeff();
            size_t totalBits = N_ * bitsPerCoeff;
            bytes_t packed((totalBits + 7) / 8, 0);

            size_t globalBit = 0;
            for (int i = 0; i < N_; ++i)
            {
                long long val = poly[i];
                for (int bit = 0; bit < bitsPerCoeff; ++bit)
                {
                    if ((val >> bit) & 1)
                    {
                        packed[globalBit / 8] |= (1 << (globalBit % 8));
                    }
                    globalBit++;
                }
            }
            return packed;
        }

        NtruPolynomial unpackCoefficients(const bytes_t& packed) const
        {
            int bitsPerCoeff = getBitsPerCoeff();
            std::vector<long long> coeffs(N_, 0);

            size_t globalBit = 0;
            for (int i = 0; i < N_; ++i)
            {
                long long val = 0;
                for (int bit = 0; bit < bitsPerCoeff; ++bit)
                {
                    if (packed[globalBit / 8] & (1 << (globalBit % 8)))
                    {
                        val |= (1LL << bit);
                    }
                    globalBit++;
                }
                coeffs[i] = val;
            }
            return NtruPolynomial(coeffs, N_, q_);
        }


        [[nodiscard]] int getBitsPerCoeff() const {
            return static_cast<int>(std::ceil(std::log2(q_)));
        }


    };
}

#endif //CRYPT_NTRU_HPP
