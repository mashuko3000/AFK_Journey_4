//
// Created by mvideo on 18/03/26.
//

#ifndef CRYPT_2_BASEPRIMALITYTEST_HPP
#define CRYPT_2_BASEPRIMALITYTEST_HPP

#include "../core_interfaces/i_primary_test.hpp"
#include "boost/random.hpp"

class BasePrimalityTest : public IPrimalityTest
{
public:
    bool isPrime(const bigint& n, double minProb) override
    {
        if(n <= 1) return false;
        if(n <= 3) return true;
        if (n % 2 == 0) return false;

        int iterations = static_cast<int>(std::ceil(std::log2(1.0 / (1.0 - minProb))));

        for (int i = 0; i < iterations; ++i)
        {
            if (!performIteration(n))
            {
                return false;
            }
        }
        return true;
    }

protected:
    virtual bool performIteration(const bigint& n) = 0;

    bigint getRandomA(const bigint& n)
    {
        static boost::random::mt19937 gen(std::time(nullptr));
        boost::random::uniform_int_distribution<bigint> dist(2, n - 2);
        return dist(gen);
    }
};

#endif //CRYPT_2_BASEPRIMALITYTEST_HPP
