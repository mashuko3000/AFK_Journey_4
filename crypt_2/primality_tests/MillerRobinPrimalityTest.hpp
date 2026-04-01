//
// Created by mvideo on 22/03/26.
//

#ifndef CRYPT_2_MILLERROBINPRIMALITYTEST_HPP
#define CRYPT_2_MILLERROBINPRIMALITYTEST_HPP

#include "BasePrimalityTest.hpp"

class MillerRabinPrimalityTest : public BasePrimalityTest
{
protected:
    double getConfidenceBase() const override{return 4.0;}
    bool performIteration(const bigint& n, const bigint& a) const override
    {
        bigint d = n - 1;
        int s = 0;
        while (!(d&1))
        {
            d >>= 1;
            s++;
        }

        bigint x = NumberTheoryService::modPow(a, d, n);
        if (x == 1 || x == n - 1) return true;

        for (int r = 1; r < s; ++r) {
            x = NumberTheoryService::modPow(x, 2, n);
            if (x == n - 1) return true;
        }
        return false;
    }
};

#endif //CRYPT_2_MILLERROBINPRIMALITYTEST_HPP
