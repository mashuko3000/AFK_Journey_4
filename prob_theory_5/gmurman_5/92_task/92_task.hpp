//
// Created by mvideo on 15/03/26.
//

#ifndef GMURMAN_5_92_TASK_HPP
#define GMURMAN_5_92_TASK_HPP

#include <vector>
#include <random>
#include <iostream>

void task_92()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> rifleDist(1, 5);
    std::uniform_real_distribution<> hitDist(0.0, 1.0);

    long long trials = 100000000;
    long long successfulHits = 0;

    for(long long i = 0; i < trials; ++i)
    {
        int rifle = rifleDist(gen);
        double hitChance;
        if(rifle <= 3)
        {
            hitChance = 0.95;
        }
        else
        {
            hitChance = 0.7;
        }
        if(hitDist(gen) <= hitChance)
        {
            successfulHits++;
        }
    }
    double empiricalProb = static_cast<double>(successfulHits) / trials;

    std::cout << "Empirical probability: " << empiricalProb << std::endl;
    std::cout << "Theoretical probability: 0.85000" << std::endl;
    std::cout << "Difference: " << std::abs(empiricalProb - 0.85) << std::endl;
}

#endif //GMURMAN_5_92_TASK_HPP
