//
// Created by maria on 27.04.2026.
//

#ifndef CRYPT_DIFFIEHELLMAN_HPP
#define CRYPT_DIFFIEHELLMAN_HPP

#include"../common/CryptoTypes.hpp"
#include "../math/NumberTheoryService.hpp"

namespace crypto
{
    class DiffieHellman
    {
    private:
        bigint p;
        bigint g;
        bigint PrivateKey;
        bigint PublicKey;
        bigint SharedKey;

    public:

        DiffieHellman(bigint prime,
                      bigint generator,
                      bigint privKey)
                      :
                      p(std::move(prime)),
                      g(std::move(generator)),
                      PrivateKey(std::move(privKey))
        {
            PublicKey = NumberTheoryService::modPow(g, PrivateKey, p);
        }

        DiffieHellman() = default;

        ~DiffieHellman() = default;

        void initFromPeer(
                const bigint& prime,
                const bigint& generator,
                const bigint& privKey
                );

        bigint getP() const;
        bigint getG() const;
        bigint getPublicKey() const {return PublicKey;}
        void computeSharedSecret(
                const bigint& peerPubKey
                );

        std::vector<uint8_t> deriveBytes(size_t count,
                                         size_t offset);
    };
}

#endif //CRYPT_DIFFIEHELLMAN_HPP
