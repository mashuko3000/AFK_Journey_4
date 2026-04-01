//
// Created by maria on 30.03.2026.
//

#ifndef CRYPT_2_RSA_TYPES_H
#define CRYPT_2_RSA_TYPES_H

#include<boost/multiprecision/cpp_int.hpp>

using bigint = boost::multiprecision::cpp_int;

struct RSAKeyPair final
{
    bigint n;
    bigint e;
    bigint d;
    bigint p;
    bigint q;
};

#endif //CRYPT_2_RSA_TYPES_H
