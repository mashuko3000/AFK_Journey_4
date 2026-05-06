//
// Created by maria on 05.05.2026.
//

#ifndef CRYPT_ELGAMALSIGNATURE_HPP
#define CRYPT_ELGAMALSIGNATURE_HPP

#include <memory>
#include <string>
#include <ctime>
#include <functional>

#include <openssl/sha.h>
#include <boost/random.hpp>

#include "../core/SignatureBase.hpp"
#include "../core/AsymmetricTraits.hpp"
#include "../math/NumberTheoryService.hpp"


namespace crypto
{
    class ElGamalSignature : public SignatureBase<ElGamalTraits>
    {
    public:
        ElGamalSignature() = default;
        std::string name() const override {return "ElGamal Signature";}

    protected:
        bytes_t signImpl(const PrivData &priv, const bytes_t &message) const override
        {
            bigint m = hashMessage(message, priv.p);

            static boost::random::mt19937 gen(static_cast<unsigned>(std::time(nullptr)));
            boost::random::uniform_int_distribution<bigint> dist(2, priv.p - 2);

            bigint k, k_inv, gcd_val, dummy_y, r, s;

            do
            {
                do
                {
                    k = dist(gen);
                    gcd_val = NumberTheoryService::extended_gcd(k, priv.p - 1, k_inv, dummy_y);
                } while (gcd_val != 1);

                if (k_inv < 0) k_inv += (priv.p - 1);

                bigint g = 2;
                r = NumberTheoryService::modPow(g, k, priv.p);

                bigint xr = (priv.x * r) % (priv.p - 1);
                bigint m_minus_xr = (m - xr) % (priv.p - 1);

                if (m_minus_xr < 0) m_minus_xr += (priv.p - 1);

                s = (m_minus_xr * k_inv) % (priv.p - 1);
            } while (s == 0);

            return serialize(r, s);
        }

        bool verifyImpl(const PubData &pub, const bytes_t &message, const bytes_t &signature) const override
        {
            bigint r, s;
            if (!deserialize(signature, r, s)) return false;

            if (r <= 0 || r >= pub.p || s <= 0 || s >= (pub.p - 1)) return false;

            bigint m = hashMessage(message, pub.p);

            bigint yr = NumberTheoryService::modPow(pub.y, r, pub.p);
            bigint rs = NumberTheoryService::modPow(r, s, pub.p);
            bigint v1 = (yr * rs) % pub.p;

            bigint v2 = NumberTheoryService::modPow(pub.g, m, pub.p);

            return v1 == v2;
        }

    private:
        bigint hashMessage(const bytes_t &msg, const bigint &p) const
        {
            unsigned char hash[SHA256_DIGEST_LENGTH];

            SHA256(msg.data(), msg.size(), hash);

            bigint h = 0;
            for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
            {
                h = (h << 8) | hash[i];
            }

            return h % (p - 1);
        }

        bytes_t serialize(const bigint &r, const bigint &s) const
        {
            std::string combined = r.str() + ":" + s.str();
            return bytes_t(combined.begin(), combined.end());
        }

        bool deserialize(const bytes_t &sig, bigint &r, bigint &s) const
        {
            std::string str(sig.begin(), sig.end());
            size_t pos = str.find(':');
            if (pos == std::string::npos) return false;
            try
            {
                r = bigint(str.substr(0, pos));
                s = bigint(str.substr(pos + 1));
                return true;
            }
            catch (...) {
                return false;
            }
        }
    };
}

#endif //CRYPT_ELGAMALSIGNATURE_HPP
