//
// Created by mvideo on 14/03/26.
//

#ifndef INC_3_HW_5_TASK_HPP
#define INC_3_HW_5_TASK_HPP

#include "../3_task/3_task.hpp"

void fifth_task()
{
    const double eps = 1e-9;
    solveMPI([](double x)
    {
        return std::pow(2, x - 1);
    }, 0.5, eps, "Point (a)");

    solveMPI([](double x)
             {
                 return std::pow(2, x - 1);
             }, -500.5, eps, "Point (a)");


    solveMPI([](double x)
    {
        return std::exp(2 * x) - 1;
    }, -0.1, eps, "Point (b)");

    double A = 2.0;
    solveMPI([A](double x) { return A - std::log(x); }, 1.5, eps, "Point (c)");

    double alpha = 1.0, beta = 0.5;
    solveMPI([alpha, beta](double x)
    {
        return alpha * std::exp(-x) + beta * x;
    }, 1.0, eps, "Point (d)");
}

#endif //INC_3_HW_5_TASK_HPP
