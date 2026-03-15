//
// Created by mvideo on 15/03/26.
//

#ifndef GMURMAN_5_102_TASK_HPP
#define GMURMAN_5_102_TASK_HPP

void task_102()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    const long long trials = 1000000;
    long long recognized_standard = 0;
    long long by_second_expert = 0;

    for (long long i = 0; i < trials; ++i)
    {
        bool first_expert = (dis(gen) < 0.55);
        double recogniton_chance = first_expert ? 0.9 : 0.98;

        if (dis(gen) < recogniton_chance)
        {
            recognized_standard++;
            if (!first_expert)
            {
                by_second_expert++;
            }
        }
    }

    double empirical_prob = (double)by_second_expert / recognized_standard;

    std::cout << "Empirical probability: " << empirical_prob << std::endl;
    std::cout << "Theoretical probability: 0.47115" << std::endl;
    std::cout << "Difference: " << std::abs(empirical_prob - 0.47115) << std::endl;

}

#endif //GMURMAN_5_102_TASK_HPP
