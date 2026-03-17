//
// Created by mvideo on 16/03/26.
//

#include "PoissonFamilyCalculator.hpp"

#include <cmath>
#include <stdexcept>

PoissonFamilyCalculator::PoissonFamilyCalculator(
        double lambda) : lambda(lambda)
{
    if(std::abs(lambda) < 1e-10) throw std::runtime_error("Lambda shoul be more than 0.0");
}

double PoissonFamilyCalculator::getPoissonProb(int k, double lam) const {
    if (k < 0) return 0.0;
    double prob = std::exp(-lam);
    for(int i = 1; i <= k; ++i)
    {
        prob *= (lam / static_cast<double>(i));
    }
    return prob;
}

double PoissonFamilyCalculator::calculateProbGreaterThanM(int m, double lam) const {
    if(m < 0) return 0.0;
    double probLessThanM = 0.0;
    for(int i = 0; i < m; ++i)
    {
        probLessThanM += getPoissonProb(i, lam);
    }
    return 1.0 - probLessThanM;
}

double PoissonFamilyCalculator::getConditionalProb(int m) const
{
    return calculateProbGreaterThanM(m, lambda/2.0);
}

double PoissonFamilyCalculator::getUnconditionalProb(int m) const
{
    return calculateProbGreaterThanM(m, lambda);
}

