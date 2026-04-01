//
// Created by mvideo on 18/03/26.
//

#ifndef CRYPT_2_I_PRIMARY_TEST_HPP
#define CRYPT_2_I_PRIMARY_TEST_HPP

#include "../helpers/NumberTheoryService.hpp"
#include <boost/multiprecision/cpp_dec_float.hpp>

using bigfloat = boost::multiprecision::cpp_dec_float_50;

class IPrimalityTest
{
public:
    virtual ~IPrimalityTest() noexcept = default;
    virtual bool isPrime(const bigint&n, bigfloat minProb) const = 0;
};

#endif //CRYPT_2_I_PRIMARY_TEST_HPP
