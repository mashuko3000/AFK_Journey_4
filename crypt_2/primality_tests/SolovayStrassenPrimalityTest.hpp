//
// Created by mvideo on 22/03/26.
//

#ifndef CRYPT_2_SOLOVAYSTRASSENPRIMALITYTEST_HPP
#define CRYPT_2_SOLOVAYSTRASSENPRIMALITYTEST_HPP

#include "BasePrimalityTest.hpp"

class SolovayStrassenPrimalityTest : public BasePrimalityTest
{
protected:
    double getConfidenceBase() const override {return 2.0;}

    bool performIteration(const bigint& n, const bigint& a) override
    {
        bigint a_copy = a;
        bigint n_copy = n;
        bigint jacobi = NumberTheoryService::jacobiSym(a_copy, n_copy);

        if (jacobi == 0) return false;

        bigint x = NumberTheoryService::modPow(a, (n - 1) / 2, n);
        bigint j_mod = (jacobi % n + n) % n;

        return x == j_mod;
    }
};


#endif //CRYPT_2_SOLOVAYSTRASSENPRIMALITYTEST_HPP
