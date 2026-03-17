//
// Created by mvideo on 16/03/26.
//

#ifndef FAMILY_INTERFACES_HPP
#define FAMILY_INTERFACES_HPP

#include "types.hpp"

class ITheoreticalCalculator
{
public:
    virtual ~ITheoreticalCalculator() = default;
    virtual double getUnconditionalProb(int m) const = 0;
    virtual double getConditionalProb(int m) const = 0;
};

class IFamilySimulator
{
public:
    virtual ~IFamilySimulator() = default;
    virtual FamilyResult simulateFamily() = 0;
};

#endif //FAMILY_INTERFACES_HPP
