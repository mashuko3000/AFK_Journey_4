//
// Created by mvideo on 14/03/26.
//

#ifndef INC_3_HW_7_TASK_HPP
#define INC_3_HW_7_TASK_HPP

#include<cmath>
#include<iostream>
#include<iomanip>
#include<vector>

const double PI = acos(-1.0);

double f(double x)
{
    return pow(x - 1.0, 3) * sin(PI * x) * (cos(2.0 * PI * x) - 1.0);
}

double df(double x)
{
    double u = pow(x - 1.0, 3);
    double v = sin(PI * x);
    double w = cos(2.0 * PI * x) - 1.0;

    double du = 3.0 * pow(x - 1.0, 2);
    double dv = PI * cos(PI * x);
    double dw = -2.0 * PI * sin(2.0 * PI * x);

    return du * v * w + u * dv * w + u * v * dw;
}

void newtonMethod(double x0, double eps, int rootIdx) {
    double x = x0;
    double x_prev;
    int iter = 0;

    std::cout << "\n Newton Method for Root " << rootIdx
         << " (x0 = " << x0 << ") ---\n";
    std::cout << std::setw(5) << "Iter"
         << std::setw(18) << "x_n"
         << std::setw(18) << "|x_n - x_{n-1}|" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    do
    {
        x_prev = x;
        double derivative = df(x);

        if (abs(derivative) < 1e-15)
        {
            std::cout << "Error: derivative is zero!\n";
            break;
        }

        x = x - f(x) / derivative;
        iter++;

        std::cout << std::setw(5) << iter
             << std::setw(18) << std::fixed << std::setprecision(10) << x
             << std::setw(18) << abs(x - x_prev) << std::endl;

    }
    while (abs(x - x_prev) > eps && iter < 1000);

    std::cout << "Final root: " << x << "\n";
}

void seventh_task()
{
    int n;
    std::cout << "Enter n for accuracy 10^-n: ";
    std::cin >> n;
    double eps = pow(10.0, -n);

    std::cout << "=== TASK 7: Newton Method === e = 10^-" << n << " ===\n";

    std::vector<double> initial_guesses = {1.1, 2.1, 3.1};

    for (int i = 0; i < initial_guesses.size(); ++i)
    {
        newtonMethod(initial_guesses[i], eps, i + 1);
    }
}

#endif //INC_3_HW_7_TASK_HPP
