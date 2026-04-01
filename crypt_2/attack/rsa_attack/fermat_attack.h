//
// Created by maria on 30.03.2026.
//

#ifndef CRYPT_2_FERMAT_ATTACK_H
#define CRYPT_2_FERMAT_ATTACK_H

#include "../../core_interfaces/i_factorization_attack.h"

class FermatAttack final : public IFactorizationAttack
{
public:
    ~FermatAttack() override = default;
    [[nodiscard]] virtual std::pair<bigint, bigint> execute(const bigint& n, const bigint& e = 0) const override;

private:
    bool perfectSquare(const bigint& x, bigint& out_root) const noexcept;
};


#endif //CRYPT_2_FERMAT_ATTACK_H
