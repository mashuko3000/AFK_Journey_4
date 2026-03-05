//
// Created by mvideo on 01/03/26.
//

#include "random_generator.hpp"

#include<cmath>
#include<ctime>
#include<stdexcept>

void random_generator::init()
{
    std::random_device rd;
    gen.seed(rd());
}
double random_generator::get_double()
{
    return dist(gen);
}
int random_generator::roll_update(int modules_count)
{
    if(modules_count == 0)
    {
        throw std::invalid_argument("count of modules must be more than 0");
    }

    double roll = get_double();
    double cumulative_probability = 0.0;

    for (int k = 4; k >= 1; --k)
    {
        double p_k = 52.0 * std::pow(10, -2 - k);
        double total_p_k = p_k * modules_count;

        cumulative_probability += total_p_k;

        if (roll < cumulative_probability)
        {
            return k;
        }
    }
    return 0;
}