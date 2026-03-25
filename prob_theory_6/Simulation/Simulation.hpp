//
// Created by mvideo on 25/03/26.
//

#ifndef PROB_THEORY_6_SIMULATION_HPP
#define PROB_THEORY_6_SIMULATION_HPP

#include"../Core/IExperiment.hpp"
#include<random>

class Simulation final : public ISimulation
{
private:
    const double PI = std::acos(-1.0);
    std::vector<NeedleResult> results;
    int crossedCount = 0;

public:
    ~Simulation() override = default;
    void run(int count, double d, double L)override
    {
        if(L>d) L=d;

        results.clear();
        results.reserve(count);
        crossedCount = 0;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> distX(0.0, d / 2.0);
        std::uniform_real_distribution<double> distPhi(0.0, M_PI / 2.0);

        for (int i = 0; i < count; ++i)
        {
            double x = distX(gen);
            double phi = distPhi(gen);

            bool crossed = x <= (L/2.0)*std::sin(phi);
            if(crossed)
            {
                crossedCount++;
            }
            results.push_back({x, phi, crossed});
        }
    }
    double getEmpiricalProb() const override
    {
        if(results.empty()) return 0.0;
        return static_cast<double>(crossedCount) / results.size();
    }
    double getTheoryProb(double d, double L) const override
    {
        if(d <= 0.0) return 0.0;
        if (L > d) L = d;
        return (2.0 * L) / (M_PI * d);
    }
    const std::vector<NeedleResult>& getResult() const override
    {
        return results;
    }
};

#endif //PROB_THEORY_6_SIMULATION_HPP
