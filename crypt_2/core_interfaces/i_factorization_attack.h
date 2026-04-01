//
// Created by maria on 30.03.2026.
//

#ifndef CRYPT_2_I_FACTORIZATION_ATTACK_H
#define CRYPT_2_I_FACTORIZATION_ATTACK_H

#include "crypto_enums.hpp"
#include"rsa_types.h"

class IFactorizationAttack
{
public:
    virtual ~IFactorizationAttack() = default;
    [[nodiscard]] virtual std::pair<bigint, bigint> execute(const bigint& n, const bigint& e = 0) const = 0;
};

#endif //CRYPT_2_I_FACTORIZATION_ATTACK_H
