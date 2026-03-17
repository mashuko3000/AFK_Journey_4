//
// Created by mvideo on 16/03/26.
//

#ifndef FAMILY_EMPIRICALANALYZER_HPP
#define FAMILY_EMPIRICALANALYZER_HPP

#include "../interfaces.hpp"

class EmpiricalAnalyzer
{
private:
    IFamilySimulator& simulator;
    long long trials;

public:
    EmpiricalAnalyzer(IFamilySimulator& sim,
                      long long trials);

    EmpiricalResults analyze(int m);
};


#endif //FAMILY_EMPIRICALANALYZER_HPP
