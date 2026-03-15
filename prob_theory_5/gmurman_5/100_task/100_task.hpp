//
// Created by mvideo on 15/03/26.
//

#ifndef GMURMAN_5_100_TASK_HPP
#define GMURMAN_5_100_TASK_HPP

#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

void task_100() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    const long long trials = 1000000;
    long long total_errors = 0;
    long long errors_by_first = 0;

    for (long long i = 0; i < trials; ++i) {
        bool first_worker = dis(gen) < 0.5;
        double error_chance = first_worker ? 0.05 : 0.1;

        if (dis(gen) < error_chance)
        {
            total_errors++;
            if (first_worker)
            {
                errors_by_first++;
            }
        }
    }

    double empirical_prob = (double)errors_by_first / total_errors;

    std::cout << "Empirical probability: " << empirical_prob << std::endl;
    std::cout << "Theoretical probability: 0.3333" << std::endl;
    std::cout << "Difference: " << std::abs(empirical_prob - 0.3333) << std::endl;

}

#endif //GMURMAN_5_100_TASK_HPP
