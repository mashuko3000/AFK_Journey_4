//
// Created by mvideo on 24/03/26.
//

#ifndef PROB_THEORY_6_RANDOMGENERATOR_HPP
#define PROB_THEORY_6_RANDOMGENERATOR_HPP

#include <random>
#include<stdexcept>

class RandomGenerator
{
private:
    static inline std::mt19937 generator;
public:
    static void seed(unsigned int s) {generator.seed(s);}

    static bool bernoulli(double prob)
    {
        if(prob >= 1.0) return true;
        if(prob <= 0.0) return false;

        return std::bernoulli_distribution{prob}(generator);
    }

    static size_t uniformIndex(
            size_t n
            )
    {
        if (n == 0) throw std::invalid_argument("Cannot sample from empty set");
        return std::uniform_int_distribution<size_t>{0, n-1}(generator);
    }

    static double uniformReal(
            double min =0.0,
            double max = 1.0
            )
    {
        return std::uniform_real_distribution<double>{min,  max}(generator);
    }
};

#endif //PROB_THEORY_6_RANDOMGENERATOR_HPP
