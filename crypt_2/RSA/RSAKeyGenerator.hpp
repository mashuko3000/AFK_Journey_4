//
// Created by mvideo on 22/03/26.
//

#ifndef CRYPT_2_RSAKEYGENERATOR_HPP
#define CRYPT_2_RSAKEYGENERATOR_HPP

#include "../primality_tests/MillerRobinPrimalityTest.hpp"
#include"../helpers/NumberTheoryService.hpp"

class RSAKeyGenerator final
{
public:
    struct KeyPair
    {
        bigint n, e, d;
    };

    static KeyPair generate(int bitLength, const IPrimalityTest& test, double confidence = 0.9999)
    {
        if (bitLength < 16) throw std::invalid_argument("Bit length is too small for RSA.");
        bigint p, q, n, phi, e, d;
        int halfBits = bitLength / 2;

        p = generatePrime(halfBits, test, confidence);

        do
        {
            q = generatePrime(halfBits, test, confidence);
        } while(p == q || abs_diff(p, q) < (bigint(1) << (halfBits - 100)));

        n = p * q;
        phi = (p - 1) * (q - 1);
/*
        e = 65537;
        if(e >= phi) e = 3;

        while(NumberTheoryService::gcd(e, phi) != 1)
        {
            e += 2;
        }*/

        unsigned int k = 16;
        e = (bigint(1) << k) + 1;

        while (NumberTheoryService::gcd(e, phi) != 1)
        {
            k++;
            e = (bigint(1) << k) + 1;
            if (e >= phi)
            {
                return generate(bitLength, test, confidence);
            }
        }

        d = NumberTheoryService::reverse(e, phi);

        return {n, e, d};
    }

private:
    static bigint generatePrime(int bits, const IPrimalityTest& test, double confidence) //
    {
        static boost::random::mt19937 gen(std::time(nullptr));
        bigint min = bigint(1) << (bits - 1);
        bigint max = (bigint(1) << bits) - 1;
        boost::random::uniform_int_distribution<bigint> dist(min, max);

        while (true)
        {
            bigint num = dist(gen);
            num |= 1;

            if (test.isPrime(num, confidence))
            {
                return num;
            }
        }
    }

    static bigint abs_diff(const bigint& a, const bigint& b)
    {
        return (a > b) ? (a - b) : (b - a);
    }
};

#endif //CRYPT_2_RSAKEYGENERATOR_HPP
