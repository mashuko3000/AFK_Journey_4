//
// Created by maria on 18.04.2026.
//

#ifndef CRYPT_ASYMMETRICTRAITS_HPP
#define CRYPT_ASYMMETRICTRAITS_HPP

#include "../common/CryptoTypes.hpp"
#include "../math/NtruPolynomial.hpp"

namespace crypto
{
    struct ElGamalTraits
    {
        static constexpr const char* ALGO_NAME = "ElGamal";
        struct PublicKeyData {BigInt p, g, y;};
        struct PrivateKeyData {BigInt p, x;};
    };

    struct NTRUTraits
    {
        static constexpr const char* ALGO_NAME = "NTRU";

        struct PublicKeyData {NtruPolynomial h;};
        struct PrivateKeyData{NtruPolynomial f, fp;};
    };
}

#endif //CRYPT_ASYMMETRICTRAITS_HPP
