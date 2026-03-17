//
// Created by mvideo on 16/03/26.
//

#ifndef FAMILY_TYPES_HPP
#define FAMILY_TYPES_HPP

struct FamilyResult
{
    int childrenCount;
    int girlsCount;
};

struct EmpiricalResults
{
    double unconditionalProb = 0.0;
    double conditionalProb = 0.0;
};

#endif //FAMILY_TYPES_HPP
