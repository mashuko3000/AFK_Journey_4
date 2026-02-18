#ifndef PROB_THEORY_1_1_TASK_HPP
#define PROB_THEORY_1_1_TASK_HPP

#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>

struct stat
{
    double first = 0.0;
    double second = 0.0;
    double both = 0.0;
};

void task_1(const int N, const int64_t simulation, bool shifted, stat& emp, stat& theo);
void run_task_1();


#endif //PROB_THEORY_1_1_TASK_HPP
