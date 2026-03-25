#include <iostream>

#include"Barrel/BarrelExplosion.hpp"
#include "Coin/Coin.hpp"
#include "Gmurman/Gmurman.hpp"

#include<memory>

int main() {
    RandomGenerator::seed(42);
/*
    BarrelExplosion exp(100, 0.01, 0.1);
    std::cout << exp.runEmpirical(1000000) << std::endl;
    std::cout << exp.calculateTheoretical() << std::endl;

    Coin coin(20, 4);
    std::cout << coin.runEmpirical(1000000) << std::endl;
    std::cout << coin.calculateTheoretical() << std::endl;
*/
    std::vector<std::unique_ptr<IExperiment<std::string>>> tasks;
    tasks.push_back(std::make_unique<Task109>());
    tasks.push_back(std::make_unique<Task111>());
    tasks.push_back(std::make_unique<Task112>());
    tasks.push_back(std::make_unique<Task113>());
    tasks.push_back(std::make_unique<Task114>());
    tasks.push_back(std::make_unique<Task115>());
    tasks.push_back(std::make_unique<Task116>());
    tasks.push_back(std::make_unique<Task117>());
    tasks.push_back(std::make_unique<Task118>());

    long long trials = 1000000;

    for (const auto& task : tasks) {
        std::cout << "--- " << task->getName() << " ---" << std::endl;
        std::cout << "Theoretical : " << task->calculateTheoretical() << std::endl;
        std::cout << "Empirical   : " << task->runEmpirical(trials) << "\n\n";
    }

    return 0;
}
