//
// Created by maria on 31.03.2026.
//

#ifndef CRYPT_2_WIENER_ATTACK_H
#define CRYPT_2_WIENER_ATTACK_H

#include "../../core_interfaces/i_factorization_attack.h"
#include"i_fraction_service.h"

class WienerAttack final : public IFactorizationAttack
{
private:
    const IFractionService& fractionService;

public:
    explicit WienerAttack(const IFractionService& serv) noexcept : fractionService(serv){}
    ~WienerAttack() override = default;
    [[nodiscard]] std::pair<bigint, bigint> execute(const bigint& n, const bigint& e = 0) const override;

private:
    bool check(const bigint& sum, const bigint& n, bigint& p, bigint& q) const noexcept;
};


#endif //CRYPT_2_WIENER_ATTACK_H
