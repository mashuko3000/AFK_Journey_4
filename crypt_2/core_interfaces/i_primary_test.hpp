//
// Created by mvideo on 18/03/26.
//

#ifndef CRYPT_2_I_PRIMARY_TEST_HPP
#define CRYPT_2_I_PRIMARY_TEST_HPP

#include "../helpers/NumberTheoryService.hpp"

class IPrimalityTest
{
public:
    virtual ~IPrimalityTest() noexcept = default;
    virtual bool isPrime(const bigint&n, double minProb) = 0;
};

#endif //CRYPT_2_I_PRIMARY_TEST_HPP
