#ifndef PROB_THEORY_1_2_TASK_HPP
#define PROB_THEORY_1_2_TASK_HPP

#include <iostream>
#include<cstdint>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#include "2_task.hpp"

double theo_p_fixed(int i, int N);
double theo_p_intersection(int i, int j, int N);
double theo_p_union(int i, int j, int N);
void print_favorable_logic(int N, int i, int j);
void task_2(
        const int N,
        const int trials,
        const int i,
        const int j,
        double& p_union,
        double& p_inter);

#endif
