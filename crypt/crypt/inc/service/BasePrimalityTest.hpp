//
// Created by mvideo on 18/03/26.
//

#ifndef CRYPT_2_BASEPRIMALITYTEST_HPP
#define CRYPT_2_BASEPRIMALITYTEST_HPP

#include "../core/IPrimalityTest.hpp"
#include "boost/random.hpp"
#include <boost/multiprecision/cpp_dec_float.hpp>

class BasePrimalityTest : public IPrimalityTest
{
public:
    bool isPrime(const bigint& n, bigfloat minProb) const override
    {
        if(n <= 1) return false;//
        if(n <= 3) return true;
        if ((n & 1) == 0) return false;

        int iterations = calculateIterations(minProb);

        for (int i = 0; i < iterations; ++i)
        {
            bigint a = getRandomA(n);
            if (!performIteration(n, a))
            {
                return false;
            }
        }
        return true;
    }

    virtual ~BasePrimalityTest() = default;

protected:
    virtual bool performIteration(const bigint& n, const bigint& a) const = 0;
    virtual double getConfidenceBase() const = 0;

    int calculateIterations(bigfloat minProb) const
    {
        bigfloat epsilon = bigfloat(1.0) - minProb;
        if (epsilon <= 0) return 100;

        bigfloat invEps = bigfloat(1.0) / epsilon;
        bigfloat base = bigfloat(getConfidenceBase());

        bigfloat k = boost::multiprecision::log(invEps) / boost::multiprecision::log(base);
        return static_cast<int>(boost::multiprecision::ceil(k));
    }

    bigint getRandomA(const bigint& n) const
    {
        static boost::random::mt19937 gen(std::time(nullptr));
        boost::random::uniform_int_distribution<bigint> dist(2, n - 2);
        return dist(gen);
    }
};

#endif //CRYPT_2_BASEPRIMALITYTEST_HPP
