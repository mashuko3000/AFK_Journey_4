//
// Created by mvideo on 15/03/26.
//

#ifndef GMURMAN_5_98_TASK_HPP
#define GMURMAN_5_98_TASK_HPP

#include<string>
#include <random>
#include <iostream>

void task_98()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> rifleDist(1, 10);
    std::uniform_real_distribution<> hitDist(0.0, 1.0);

    const long long trials = 1000000;
    long long total_hits = 0;

    long long hits_from_optic = 0;
    long long hits_from_classic = 0;

    for (long long i = 0; i < trials; ++i)
    {
        int rifle = rifleDist(gen);
        bool is_optic = (rifle <= 4);
        double chance = is_optic ? 0.95 : 0.8;

        if (hitDist(gen) <= chance)
        {
            total_hits++;
            if (is_optic)
            {
                hits_from_optic++;
            }
            else
            {
                hits_from_classic++;
            }
        }
    }
    double prob_optic = (double)hits_from_optic / total_hits;
    double prob_classic = (double)hits_from_classic / total_hits;

    std::cout << "Empirical probability: " << prob_optic << std::endl;
    std::cout << "Theoretical probability: 0.44186" << std::endl;
    std::cout << "Difference: " << std::abs(prob_optic - 0.44186) << std::endl;

    std::cout << "Empirical probability: " << prob_classic << std::endl;
    std::cout << "Theoretical probability: 0.5581" << std::endl;
    std::cout << "Difference: " << std::abs(prob_classic - 0.5581) << std::endl;
}

#endif //GMURMAN_5_98_TASK_HPP
