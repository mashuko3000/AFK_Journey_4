//
// Created by maria on 27.04.2026.
//
#include "../inc/service/DiffieHellman.hpp"
using namespace crypto;

void DiffieHellman::initFromPeer(
        const bigint& prime,
        const bigint& generator,
        const bigint& privKey
)
{
    p = prime;
    g = generator;
    PrivateKey = privKey;
    PublicKey = NumberTheoryService::modPow(g, PrivateKey, p);
}

bigint DiffieHellman::getP() const
{
    return p;
}
bigint DiffieHellman::getG() const
{
    return g;
}

void DiffieHellman::computeSharedSecret(
        const bigint& peerPubKey
)
{
    if(peerPubKey <= 1 || peerPubKey >= p - 1)
    {
        throw std::runtime_error("Invalid peer public key for Diffie-Hellman");
    }
    SharedKey = NumberTheoryService::modPow(peerPubKey, PrivateKey, p);
}

std::vector<uint8_t> DiffieHellman::deriveBytes(size_t count,
                                 size_t offset)
{
    if(SharedKey == 0)
    {
        throw std::runtime_error("Shared secret has not been computed yet");
    }

    std::vector<uint8_t> result;
    bigint temp = SharedKey >> (offset * 8);
    for (size_t i = 0; i < count; ++i)
    {
        result.push_back(static_cast<uint8_t>(temp & 0xFF));
        temp >>= 8;
    }
    return result;
}