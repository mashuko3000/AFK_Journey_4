//
// Created by mvideo on 22/03/26.
//

#ifndef CRYPT_2_RSAKEYGENERATOR_HPP
#define CRYPT_2_RSAKEYGENERATOR_HPP

#include "../primality_tests/MillerRobinPrimalityTest.hpp"
#include"../helpers/NumberTheoryService.hpp"

class RSAKeyGenerator
{
public:
    struct KeyPair
    {
        bigint n, e, d;
    };

    static KeyPair generate(int bitLength) {
        MillerRabinPrimalityTest primalityTest;
        bigint p = generatePrime(bitLength / 2, primalityTest);
        bigint q = generatePrime(bitLength / 2, primalityTest);

        bigint n = p * q;
        bigint phi = (p - 1) * (q - 1);
        bigint e = 65537;
        bigint d = NumberTheoryService::reverse(e, phi);

        return {n, e, d};
    }

private:
    static bigint generatePrime(int bits, IPrimalityTest& test)
    {
        static boost::random::mt19937 gen(std::time(nullptr));
        bigint min = bigint(1) << (bits - 1);
        bigint max = (bigint(1) << bits) - 1;
        boost::random::uniform_int_distribution<bigint> dist(min, max);

        while (true) {
            bigint num = dist(gen);
            if (num % 2 == 0) num++;
            if (test.isPrime(num, 0.9999)) {
                return num;
            }
        }
    }
};

#endif //CRYPT_2_RSAKEYGENERATOR_HPP
