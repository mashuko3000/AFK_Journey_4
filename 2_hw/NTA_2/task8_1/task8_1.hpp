//
// Created by mvideo on 27/02/26.
//

#ifndef NTA_2_TASK8_1_HPP
#define NTA_2_TASK8_1_HPP

#include<iostream>
#include <iomanip>

struct complex
{
    double re, im;
};

complex seven_operation_division_mult(
        const double a0,
        const double a1,
        const double b0,
        const double b1,
        int& count
);

complex six_operation_division_mult(
        const double a0,
        const double a1,
        const double b0,
        const double b1,
        int& count
);
complex div_standard(
        const double a0,
        const double a1,
        const double b0,
        const double b1,
        int& count
);
void task_8_1(
        double a0,
        double a1,
        double b0,
        double b1
);
#endif //NTA_2_TASK8_1_HPP
