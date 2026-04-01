//
// Created by maria on 31.03.2026.
//

#ifndef CRYPT_2_FRACTIONSERVICE_H
#define CRYPT_2_FRACTIONSERVICE_H

#include "i_fraction_service.h"
#include "NumberTheoryService.hpp"

class FractionService final : public IFractionService
{
public:
    FractionService() = default;
    ~FractionService() noexcept override = default;
    [[nodiscard]] std::vector<bigint> getContinuedFraction(const Fraction& fraction) const override;
    [[nodiscard]] Fraction getFractionFromContinued(const std::vector<bigint>& cf) const override;
    [[nodiscard]] std::vector<Fraction> getConvergents(const Fraction& fraction)  const override;

    [[nodiscard]] Path getCalkinWilfPath(const Fraction& fraction) const override;
    [[nodiscard]] Path getSternBrocotPath(const Fraction& fraction) const override;

    [[nodiscard]] Fraction getFractionFromCalkinWilfPath(const Path& path) const override;
    [[nodiscard]] Fraction getFractionFromSternBrocotPath(const Path& path) const override;

    [[nodiscard]] std::vector<Fraction> getConvergentsFromSternBrocotPath(const Path& path) const override;

private:
    void validateFraction(const Fraction& f) const;
    [[nodiscard]] std::vector<Fraction> generateConvergentsFromCF(const std::vector<bigint>& cf) const;
};


#endif //CRYPT_2_FRACTIONSERVICE_H
