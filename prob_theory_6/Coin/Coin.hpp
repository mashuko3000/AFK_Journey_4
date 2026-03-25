//
// Created by mvideo on 24/03/26.
//

#ifndef PROB_THEORY_6_COIN_HPP
#define PROB_THEORY_6_COIN_HPP

#include"../Core/IExperiment.hpp"
#include"../Core/RandomGenerator.hpp"

class Coin final: public IExperiment<double>
{
private:
    size_t totalCoins;
    size_t headedCoins;
    
public:
    Coin(size_t t,
         size_t h)
         :
         totalCoins(t),
         headedCoins(h)
    {
        if(t < h) throw std::invalid_argument("Double-headed coins cannot exceed total coins");
    }
    ~Coin() override = default;

    double runEmpirical(long long trials) override
    {
        size_t success = 0;
        size_t condition = 0;

        for(long long i = 0; i < trials; ++i)
        {
            size_t coinIDX = RandomGenerator::uniformIndex(totalCoins);
            bool isDouble = (coinIDX < headedCoins);

            bool three_heads = true;
            for(int j = 0; j < 3; ++j)
            {
                if(!isDouble && !RandomGenerator::bernoulli(0.5))
                {
                    three_heads = false;
                    break;
                }
            }

            if(three_heads)
            {
                ++condition;
                bool fourth = isDouble || RandomGenerator::bernoulli(0.5);
                if(fourth) ++success;
            }
        }
        return condition == 0 ? 0.0 : static_cast<double>(success) / condition;
    }

    double calculateTheoretical() const override
    {
        double n = totalCoins;
        double k = headedCoins;
        double p3 = (k / n) * 1.0 + ((n - k) / n) * (1.0 / 8.0);
        double p4 = (k / n) * 1.0 + ((n - k) / n) * (1.0 / 16.0);
        return p4 / p3;
    }
    std::string getName() const override
    {
        return "Task 2: Conditional Coin Flips";
    }
};

#endif //PROB_THEORY_6_COIN_HPP
