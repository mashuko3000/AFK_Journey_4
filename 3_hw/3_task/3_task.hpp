//
// Created by mvideo on 13/03/26.
//

#ifndef INC_3_HW_3_TASK_HPP
#define INC_3_HW_3_TASK_HPP

#include <functional>
#include<iomanip>
#include<iostream>
#include <string>
#include <cmath>

double derivate(std::function<double(double)> f, double x)
{
    double h = 1e-9;
    return (f(x+h) - f(x-h)) / (2.0 * h);
}

void solveMPISmart(std::function<double(double)> f,
              double x0,
              double eps,
              std::string rootName)
{
    double df = derivate(f, x0);
    double lambda = (abs(df) > 1e-3) ? (-1.0/df) : 0.5;

    std::cout << "\nlambda is : " << lambda << std::endl;

    auto phi = [f, lambda](double x)
    {
        return x + lambda * f(x);
    };

    double x = x0;
    int iter = 0;
    while(true)
    {
        double x_next = phi(x);
        double diff = std::abs(x_next - x);
        x = x_next;
        if(diff < eps || iter > 500) break;
    }
    std::cout << "Root is " << x << " [Iteration :" << iter << " ]" << std::endl;
}

void solveMPI(const std::function<double(double)> phi,
              double x0,
              const double eps,
              const std::string rootName)
{
    double x_curr = x0;
    int iter = 0;

    std::cout << "\n=== Solving for root: " << rootName << " ===" << std::endl;
    std::cout << std::setw(6) << "Iter" << " | " << std::setw(18) << "x_n"
              << " | " << "diff" << std::endl;
    std::cout << std::string(53, '-') << std::endl;

    while(true)
    {
        double x_next = phi(x_curr);
        double diff = std::abs(x_next - x_curr);

        std::cout << std::setw(6) << ++iter << " | "
                  << std::fixed << std::setprecision(10) << x_next
                  << " | " << std::scientific << std::setprecision(2) << diff << std::endl;

        if (diff < eps)
        {
            x_curr = x_next;
            break;
        }

        x_curr = x_next;

        if (iter >= 300)
        {
            std::cout << "!!! Error: Method did not converge within 300 iterations" << std::endl;
            return;
        }
    }
    std::cout << "Final root: " << std::fixed << std::setprecision(10) << x_curr
              << " (in " << iter << " iterations)" << std::endl;
}

void third_task()
{
    const double eps = 1e-8;

    std::cout << "\n\n=== EQUATION a) x^3 + 3x^2 - 1 = 0 ===";
    auto phi_a = [](double x) { return std::cbrt(1.0 - 3.0 * x * x); };

    solveMPI(phi_a, -3.0, eps, "z1 (approx -2.879)");
    solveMPI(phi_a, -0.7, eps, "z2 (approx -0.653)");
    solveMPI(phi_a,  0.3, eps, "z3 (approx  0.532)");

    std::cout << "\n\n=== EQUATION b) x^4 - x^3 = 0 ===";
    auto phi_b0 = [](double x) { return std::pow(x, 4); };
    auto phi_b1 = [](double x) { return x - (std::pow(x, 4) - std::pow(x, 3)); };

    solveMPI(phi_b0, 0.5, eps, "Root 0.0");
    solveMPI(phi_b1, 0.9, eps, "Root 1.0");

    std::cout << "\n\n=== EQUATION c) x^2 - 3x + 2 = 0 ===";
    auto f_c = [](double x) { return x * x - 3.0 * x + 2.0; };

    solveMPI([&](double x) { return x + 0.4 * f_c(x); }, 0.5, eps, "Root 1.0");
    solveMPI([&](double x) { return x - 0.4 * f_c(x); }, 2.5, eps, "Root 2.0");
}

#endif //INC_3_HW_3_TASK_HPP
