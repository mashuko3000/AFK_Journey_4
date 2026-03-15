//
// Created by mvideo on 15/03/26.
//

#ifndef GMURMAN_5_93_TASK_HPP
#define GMURMAN_5_93_TASK_HPP

#include<string>
#include <random>
#include <iostream>

struct Factory
{
    std::string name;
    int details_count;
    double quality;
};

void task_93()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::vector<Factory> factories =
    {
            {"Factory #1", 12, 0.9},
            {"Factory #2", 20, 0.6},
            {"Factory #3", 18, 0.9}
    };

    int total_details = 50;

    const long long trials = 1000000;
    long long successful = 0;

    for (long long i = 0; i < trials; ++i)
    {
        int pick = std::uniform_int_distribution<>(0, total_details - 1)(gen);

        int selected_factory_idx = 0;
        int current_sum = 0;
        for (int j = 0; j < factories.size(); ++j)
        {
            current_sum += factories[j].details_count;
            if (pick < current_sum)
            {
                selected_factory_idx = j;
                break;
            }
        }

        if (dis(gen) <= factories[selected_factory_idx].quality)
        {
            successful++;
        }
    }
    double empiricalProb = static_cast<double>(successful) / trials;

    std::cout << "Empirical probability: " << empiricalProb << std::endl;
    std::cout << "Theoretical probability: 0.7800" << std::endl;
    std::cout << "Difference: " << std::abs(empiricalProb - 0.78) << std::endl;
}

#endif //GMURMAN_5_93_TASK_HPP
