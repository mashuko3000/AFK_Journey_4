//
// Created by maria on 30.03.2026.
//

#ifndef CRYPT_2_I_CONTINUED_FRACTION_SERVICE_H
#define CRYPT_2_I_CONTINUED_FRACTION_SERVICE_H

#include "crypto_enums.hpp"
#include"rsa_types.h"

class IFractionService
{
public:
    virtual ~IFractionService() noexcept = default;
    [[nodiscard]] virtual std::vector<bigint> getContinuedFraction(const Fraction& fraction) const = 0;
    [[nodiscard]] virtual Fraction getFractionFromContinued(const std::vector<bigint>& cf) const = 0;
    [[nodiscard]] virtual std::vector<Fraction> getConvergents(const Fraction& fraction)  const = 0;

    [[nodiscard]] virtual Path getCalkinWilfPath(const Fraction& fraction) const = 0;
    [[nodiscard]] virtual Path getSternBrocotPath(const Fraction& fraction) const = 0;

    [[nodiscard]] virtual Fraction getFractionFromCalkinWilfPath(const Path& path) const = 0;
    [[nodiscard]] virtual Fraction getFractionFromSternBrocotPath(const Path& path) const = 0;

    [[nodiscard]] virtual std::vector<Fraction> getConvergentsFromSternBrocotPath(const Path& path) const = 0;
};

#endif //CRYPT_2_I_CONTINUED_FRACTION_SERVICE_H
