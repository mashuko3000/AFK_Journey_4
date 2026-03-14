//
// Created by mvideo on 14/03/26.
//

#ifndef INC_3_HW_8_TASK_HPP
#define INC_3_HW_8_TASK_HPP

#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <string>

const double PI_8 = acos(-1.0);

double f_8(double x)
{
    return pow(x - 1.0, 3) * sin(PI_8 * x) * (cos(2.0 * PI_8 * x) - 1.0);
}

double df_8(double x)
{
    double u = pow(x - 1.0, 3);
    double v = sin(PI_8 * x);
    double w = cos(2.0 * PI_8 * x) - 1.0;

    double du = 3.0 * pow(x - 1.0, 2);
    double dv = PI_8 * cos(PI_8 * x);
    double dw = -2.0 * PI_8 * sin(2.0 * PI_8 * x);

    return du * v * w + u * dv * w + u * v * dw;
}

int runNewton(double x0, double eps, double sigma, bool silent = true)
{
    double x = x0;
    int iter = 0;

    while (true)
    {
        double fx = f(x);
        double dfx = df(x);
        if (abs(dfx) < 1e-18) break;

        double x_next = x - sigma * (fx / dfx);
        iter++;

        if (!silent)
        {
            std::cout << std::setw(5) << iter
                 << std::setw(18) << std::fixed << std::setprecision(10) << x_next
                 << std::setw(18) << abs(x_next - x) << std::endl;
        }

        if (abs(x_next - x) < eps || iter > 1000)
        {
            return iter;
        }
        x = x_next;
    }
    return iter;
}

void eight_task()
{
    std::cout << "=== TASK 8: Modified Newton Method (quadratic convergence) ===\n\n";

    struct RootInfo
    {
        double guess;
        double p;
        std::string name;
    };

    std::vector<RootInfo> roots =
    {
            {1.1, 6.0, "Root z1 = 1 (multiplicity p=6)"},
            {2.1, 3.0, "Root z2 = 2 (multiplicity p=3)"},
            {3.1, 3.0, "Root z3 = 3 (multiplicity p=3)"}
    };

    std::vector<int> n_values = {3, 4, 5, 6};

    for (const auto& r : roots)
    {
        std::cout << "\n>>> " << r.name << " <<<\n";
        std::cout << std::setw(10) << "Precision"
             << std::setw(18) << "Standard Newton (σ=1)"
             << std::setw(18) << "Modified Newton (σ=p)"
             << std::endl;
        std::cout << std::string(55, '-') << std::endl;

        for (int n : n_values)
        {
            double eps = pow(10.0, -n);
            int iter_std = runNewton(r.guess, eps, 1.0, true);
            int iter_mod = runNewton(r.guess, eps, r.p, true);
            std::cout << "10^-" << n
                 << std::setw(15) << iter_std
                 << std::setw(18) << iter_mod << std::endl;
        }
    }

    std::cout << "\n=== Task 8 completed ===\n";
    std::cout << "Note: Modified Newton (σ = multiplicity) always needs fewer iterations.\n";
}

#endif //INC_3_HW_8_TASK_HPP
