//
// Created by mvideo on 16/03/26.
//

#ifndef FAMILY_POISSONFAMILYCALCULATOR_HPP
#define FAMILY_POISSONFAMILYCALCULATOR_HPP

#include "../interfaces.hpp"

class PoissonFamilyCalculator : public ITheoreticalCalculator
{
private:
    double lambda = 0.0;

public:
    explicit PoissonFamilyCalculator(double lambda);
    double getUnconditionalProb(int m) const override;
    double getConditionalProb(int m) const override;
    ~PoissonFamilyCalculator() noexcept override = default;

private:
    double getPoissonProb(int k, double lam) const;
    double calculateProbGreaterThanM(int m, double lam) const;
};


#endif //FAMILY_POISSONFAMILYCALCULATOR_HPP
