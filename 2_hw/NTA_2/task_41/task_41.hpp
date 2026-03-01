//
// Created by mvideo on 28/02/26.
//

#ifndef NTA_2_TASK_41_HPP
#define NTA_2_TASK_41_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

struct complex {
    double re;
    double im;
};
complex three_multiplication_product(
        const double a,
        const double b,
        const double c,
        const double d,
        int& count
);
complex standard_product(
        const double a0,
        const double a1,
        const double b0,
        const double b1,
        int& count
);
void print(std::string name, complex res, int ops);
void task_41(
        double a0, double a1,
        double b0, double b1
);
#endif //NTA_2_TASK_41_HPP
