//
// Created by mvideo on 16/03/26.
//

#include "EmpiricalAnalyzer.hpp"
#include <stdexcept>

EmpiricalAnalyzer::EmpiricalAnalyzer(
        IFamilySimulator &sim,
        long long trials)
        :
        simulator(sim),
        trials(trials)
{
    if(trials <= 0) throw std::invalid_argument("Amount of trials should be more than 0");
}

EmpiricalResults EmpiricalAnalyzer::analyze(int m)
{
    long long unconditionalSuccess = 0;
    long long conditionalSuccess = 0;
    long long conditionMetCount = 0;

    for( long long i =0 ; i <= trials; ++i)
    {
        FamilyResult result = simulator.simulateFamily();
        if(result.childrenCount >= m)
        {
            unconditionalSuccess++;
        }
        if(result.girlsCount == 0)
        {
            conditionMetCount++;
            if(result.childrenCount >= m)
            {
                conditionalSuccess++;
            }
        }
    }

    double uncondProb = static_cast<double>(unconditionalSuccess) / trials;
    double condProb = (conditionMetCount > 0)
            ? static_cast<double>(conditionalSuccess) / conditionMetCount
            : 0.0;

    return {uncondProb, condProb};
}
