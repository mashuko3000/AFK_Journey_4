
#ifndef CRYPT_2_NUMBERTHEORYSERVICE_HPP
#define CRYPT_2_NUMBERTHEORYSERVICE_HPP

//#include <bigmath/bigint.hpp>
#include "boost/multiprecision/cpp_int.hpp"

using bigint = boost::multiprecision::cpp_int;

class NumberTheoryService
{
public:
    static bigint legendreSymb(bigint& num, bigint& p);
    static bigint jacobiSym(bigint& num1, bigint& num2);
    static bigint gcd(bigint a, bigint b);
    static bigint extended_gcd(bigint a,
                               bigint b,
                               bigint& x,
                               bigint& y);
    static bigint modPow(bigint base,
                         bigint exp,
                         bigint mod);
    static bigint reverse(bigint a,
                          bigint b);
    static bigint eulerByFactorization(bigint n);
    static bigint eulerByDFT(bigint n);
};

#endif //CRYPT_2_NUMBERTHEORYSERVICE_HPP
