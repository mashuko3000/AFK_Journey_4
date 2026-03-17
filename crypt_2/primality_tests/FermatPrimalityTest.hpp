//
// Created by mvideo on 18/03/26.
//

#ifndef CRYPT_2_FERMATPRIMALITYTEST_HPP
#define CRYPT_2_FERMATPRIMALITYTEST_HPP

#include "BasePrimalityTest.hpp"

class FermatPrimalityTest : public BasePrimalityTest
{
protected:
    bool performIteration(const bigint& n) override
    {
        bigint a = getRandomA(n);
        if (NumberTheoryService::gcd(a, n) != 1) return false;
        return NumberTheoryService::modPow(a, n - 1, n) == 1;
    }
};

#endif //CRYPT_2_FERMATPRIMALITYTEST_HPP
