//
// Created by maria on 30.03.2026.
//

#include "VulnerableRSAService.h"
#include <boost/random.hpp>
#include<random>

[[nodiscard]] RSAKeyPair VulnerableRSAService::generateFermatVulnerable(size_t bit_length) const
{
    if (bit_length < 16) throw std::invalid_argument("Bit length too small.");
    const size_t half_bits = bit_length / 2;

    const bigint p = primeGenerator.generate_random_prime(half_bits);
    const bigint q = primeGenerator.find_next_prime(p + 2);

    const bigint n = p * q;
    const bigint phi = (p - 1) * (q - 1);

    bigint e = 65537;
    while (NumberTheoryService::gcd(e, phi) != 1)
    {
        e += 2;
    }

    const bigint d = NumberTheoryService::reverse(e, phi);

    return {n, e, d, p, q};
}
[[nodiscard]] RSAKeyPair VulnerableRSAService::generateWienerVulnerable(size_t bitLength) const
{
    if (bitLength < 16) throw std::invalid_argument("Bit length too small.");
    static boost::random::mt19937 gen(std::random_device{}());

    bigint p, q, n, phi, e, d;
    int halfBits = bitLength / 2;
    do
    {
        p = primeGenerator.generate_random_prime(halfBits);
        q = primeGenerator.generate_random_prime(halfBits);
        if(p < q) std::swap(p, q);
    } while(p == q || p>=2*q);

    n = p * q;
    phi = (p - 1) * (q - 1);

    size_t dBits = bitLength / 4;
    if(dBits > 2) dBits -= 1;

    bigint min_d = bigint(1) << (dBits > 1 ? dBits - 1 : 1);
    bigint max_d = (bigint(1) << dBits) - 1;
    boost::random::uniform_int_distribution<bigint> dist(min_d, max_d);

    while (true)
    {
        d = dist(gen);
        d |= 1;
        if(NumberTheoryService::gcd(d, phi) == 1)
        {
            try
            {
                e = NumberTheoryService::reverse(d, phi);
                if(e > 0)
                {
                    break;
                }
            }
            catch(const std::invalid_argument&)
            {
                continue;
            }
        }
    }
    return {n, e, d, p, q};
}
