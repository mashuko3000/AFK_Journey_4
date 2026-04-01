//
// Created by maria on 30.03.2026.
//

#ifndef CRYPT_2_PRIMEGENERATOR_H
#define CRYPT_2_PRIMEGENERATOR_H

#include "../core_interfaces/i_primary_test.hpp"
#include <boost/random.hpp>
#include <ctime>
#include <stdexcept>

class PrimeGenerator final
{
private:
    const IPrimalityTest& test;
    double confidence;

public:
    explicit PrimeGenerator(const IPrimalityTest& test, double confidence = 0.9999) noexcept
            : test(test), confidence(confidence)
    {
        if(confidence < 0.0) confidence = 0.0;
        if(confidence > 1.0) confidence = 1.0;
    }

    [[nodiscard]] bigint generate_random_prime(size_t bits) const
    {
        if (bits < 2) throw std::invalid_argument("Bits must be at least 2.");

        static boost::random::mt19937 gen(std::time(nullptr));
        const bigint min = bigint(1) << (bits - 1);
        const bigint max = (bigint(1) << bits) - 1;
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

    [[nodiscard]] bigint find_next_prime(const bigint& start) const
    {
        bigint candidate = start;
        if ((candidate & 1) == 0) candidate += 1;

        while (!test.isPrime(candidate, confidence))
        {
            candidate += 2;
        }
        return candidate;
    }
};

#endif //CRYPT_2_PRIMEGENERATOR_H
