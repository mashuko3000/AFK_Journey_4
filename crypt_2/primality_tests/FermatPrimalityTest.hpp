//
// Created by mvideo on 18/03/26.
//

#ifndef CRYPT_2_FERMATPRIMALITYTEST_HPP
#define CRYPT_2_FERMATPRIMALITYTEST_HPP

#include "BasePrimalityTest.hpp"

class FermatPrimalityTest : public BasePrimalityTest
{
protected:
    double getConfidenceBase() const override {return 2.0;}
    bool performIteration(const bigint& n, const bigint& a) const override
    {
        bigint g = NumberTheoryService::gcd(a, n);
        bigint res = NumberTheoryService::modPow(a, n - 1, n);

        if (g != 1) return false;
        return res == 1;
    }
};

#endif //CRYPT_2_FERMATPRIMALITYTEST_HPP
