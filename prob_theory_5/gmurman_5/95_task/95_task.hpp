//
// Created by mvideo on 15/03/26.
//

#ifndef GMURMAN_5_95_TASK_HPP
#define GMURMAN_5_95_TASK_HPP

#include<string>
#include <random>
#include <iostream>

enum Ball { BLACK, WHITE };

class Urn {
public:
    std::vector<Ball> balls;

    Urn(int black, int white)
    {
        for (int i = 0; i < black; ++i) balls.push_back(BLACK);
        for (int i = 0; i < white; ++i) balls.push_back(WHITE);
    }

    Ball drawAndRemove(std::mt19937& gen)
    {
        std::uniform_int_distribution<> dis(0, balls.size() - 1);
        int idx = dis(gen);
        Ball b = balls[idx];
        balls.erase(balls.begin() + idx);
        return b;
    }

    void addBall(Ball b)
    {
        balls.push_back(b);
    }
};

void task_95()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    const long long trials = 1000000;
    long long successful = 0;

    for (long long i = 0; i < trials; ++i)
    {
        Urn urn1(6, 4);
        Urn urn2(6, 4);
        Urn urn3(6, 4);

        Ball ball1to2 = urn1.drawAndRemove(gen);
        urn2.addBall(ball1to2);

        Ball ball2to3 = urn2.drawAndRemove(gen);
        urn3.addBall(ball2to3);

        Ball finalBall = urn3.drawAndRemove(gen);

        if (finalBall == WHITE) {
            successful++;
        }
    }
    double empiricalProb = static_cast<double>(successful) / trials;

    std::cout << "Empirical probability: " << empiricalProb << std::endl;
    std::cout << "Theoretical probability: 0.4000" << std::endl;
    std::cout << "Difference: " << std::abs(empiricalProb - 0.4) << std::endl;
}

#endif //GMURMAN_5_95_TASK_HPP
