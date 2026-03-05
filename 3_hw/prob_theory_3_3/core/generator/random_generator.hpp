//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_3_RANDOM_GENERATOR_HPP
#define PROB_THEORY_3_3_RANDOM_GENERATOR_HPP

#include<random>

class random_generator {
private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist;
public:
    void init();
    double get_double();
    int roll_update(int modules_count);
    explicit random_generator(unsigned int seed = std::random_device{}()) : gen(seed), dist(0.0, 1.0){};
};


#endif //PROB_THEORY_3_3_RANDOM_GENERATOR_HPP
