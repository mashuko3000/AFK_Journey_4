//
// Created by mvideo on 15/03/26.
//

#ifndef GMURMAN_5_107_TASK_HPP
#define GMURMAN_5_107_TASK_HPP

#include <iostream>
#include <vector>
#include <random>

void task_107()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    const long long trials = 1000000;
    long long exactly_two_hits = 0;
    long long third_hit_in_two = 0;

    for (long long i = 0; i < trials; ++i)
    {
        bool hit1 = dis(gen) < 0.6;
        bool hit2 = dis(gen) < 0.5;
        bool hit3 = dis(gen) < 0.4;

        int hits = (hit1 ? 1 : 0) + (hit2 ? 1 : 0) + (hit3 ? 1 : 0);

        if (hits == 2)
        {
            exactly_two_hits++;
            if (hit3)
            {
                third_hit_in_two++;
            }
        }
    }

    double empirical_prob = (double)third_hit_in_two / exactly_two_hits;

    std::cout << "Empirical probability: " << empirical_prob << std::endl;
    std::cout << "Theoretical probability: 0.5263" << std::endl;
    std::cout << "Difference: " << std::abs(empirical_prob - 0.5263) << std::endl;
}


#endif //GMURMAN_5_107_TASK_HPP
