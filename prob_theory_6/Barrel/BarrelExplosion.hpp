//
// Created by mvideo on 24/03/26.
//

#ifndef PROB_THEORY_6_BARRELEXPLOSION_HPP
#define PROB_THEORY_6_BARRELEXPLOSION_HPP

#include"../Core/IExperiment.hpp"
#include"../Core/RandomGenerator.hpp"

#include<stdexcept>

class BarrelExplosion final : public IExperiment<double>
{
private:
    double probHit;
    double probExpl;
    size_t shots;

public:
    BarrelExplosion(
            size_t n,
            double hit,
            double expl
            )
            :
            shots(n),
            probHit(hit),
            probExpl(expl)
    {
        if(hit < 0 || expl < 0 || hit > 1 || expl > 1)
        {
            throw std::invalid_argument("Probabilities must be in [0,1]");
        }
    }

    ~BarrelExplosion() override = default;

    std::string getName() const override
    {
        return "Task 1: Barrel Explosion";
    }

    double runEmpirical(long long trials) override
    {
        if(trials == 0) return 0.0;
        long long explosion = 0;
        for(long long i = 0; i < trials; ++i)
        {
            long long hits = 0;
            for(size_t s = 0; s < shots; ++s)
            {
                if(RandomGenerator::bernoulli(probHit)) ++hits;
            }
            if(hits >= 2 || (hits == 1 && RandomGenerator::bernoulli(probExpl)))
            {
                ++explosion;
            }
        }
        return static_cast<double>(explosion)/trials;
    }

    double calculateTheoretical() const override
    {
        double q = 1 - probHit;
        double q0 = std::pow(q, shots - 1);
        double p0 = q0 * q;
        double p1 = shots * probHit * q0;
        double p2plus = 1.0 - p0 - p1;

        return p1 * probExpl + p2plus;
    }
};

#endif //PROB_THEORY_6_BARRELEXPLOSION_HPP
