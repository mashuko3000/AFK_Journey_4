#include <iostream>
#include <iomanip>

#include"EmpiricalAnalyzer/EmpiricalAnalyzer.hpp"
#include "PoissonFamilySimulator/PoissonFamilySimulator.hpp"
#include"PoissonFamilyCalculator/PoissonFamilyCalculator.hpp"
#include "math/distribution.hpp"

int main()
{
    try
    {
        const double lambda = 1.94;
        const int m = 2;
        const long long trials = 10000000;

        auto children_dist = std::make_unique<poisson_distribution>(lambda);
        auto gender_dist = std::make_unique<uniform_distribution_model>(2);

        PoissonFamilyCalculator calc(lambda);
        PoissonFamilySimulator sim(std::move(children_dist), std::move(gender_dist));
        EmpiricalAnalyzer analyzer(sim, trials);

        double theoryUncond = calc.getUnconditionalProb(m);
        double theoryCond = calc.getConditionalProb(m);

        auto empRes = analyzer.analyze(m);

        std::cout << std::fixed << std::setprecision(6);
        std::cout << "=== Theory probability ===\n";
        std::cout << "Unconditional (children >= " << m << "): " << theoryUncond << "\n";
        std::cout << "Conditional (children >= " << m << " | girls = 0): " << theoryCond << "\n\n";

        std::cout << "=== Empirical Probability ===\n";
        std::cout << "Unconditional: " << empRes.unconditionalProb << "\n";
        std::cout << "Conditional: " << empRes.conditionalProb << "\n\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }
    return 0;
}
