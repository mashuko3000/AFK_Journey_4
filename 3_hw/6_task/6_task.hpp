//
// Created by mvideo on 14/03/26.
//

#ifndef INC_3_HW_6_TASK_HPP
#define INC_3_HW_6_TASK_HPP

#include<functional>
#include<string>
#include<iostream>
#include <iomanip>
#include<set>
#include<cmath>
#include <vector>

void dichotomy(std::function<double(double)> f,
               double left,
               double right,
               int n,
               std::string name)
{
    std::cout << "\n === " << name << " === " << std::endl;
    std::cout << " DICHOTOMY " << std::endl;
    std::cout << "Iteration | Approximation" << std::endl;
    std::cout << std::string(30, '-') << std::endl;

    double a = left, b = right;
    double eps = 1e-9;
    int iter = 0;

    while((b - a) / 2 > eps)
    {
        double c = (a + b) / 2.0;
        iter++;
        std::cout << std::setw(9) << iter << " | " << std::fixed << std::setprecision(12) << c << std::endl;

        if (f(a) * f(c) < 0) b = c;
        else a = c;
    }
    std::cout << "Root = " << ((a+b)/2) << " (iter : " << iter << ")" << std::endl;
}

std::pair<double, int> run_newton(std::function<double(double)> f,
                                  std::function<double(double)> df,
                                  double x0,
                                  int n,
                                  std::string name)
{
    double eps = std::pow(10.0, -n);
    double x = x0;
    int iter = 0;

    std::cout << "\nNEWTON    | " << name << " (e = 10^-" << n << ", x0 = " << x0 << ")\n";
    std::cout << "Iter | Approximation\n";
    std::cout << "-------------------\n";

    while(true)
    {
        double fx = f(x);
        double dfx = df(x);
        if (std::abs(dfx) < 1e-14)
        {
            std::cout << "Derivative ~ 0! Stopping.\n";
            break;
        }

        double x_next = x - fx / dfx;
        iter++;
        std::cout << std::setw(4) << iter
        << " | " << std::fixed << std::setprecision(12)
        << x_next << std::endl;

        if (std::abs(x_next - x) < eps)
        {
            std::cout << "Root ~ " << x_next << "   (iterations: " << iter << ")\n";
            return {x_next, iter};
        }

        x = x_next;
        if (iter > 100) break;
    }
    return {x, iter};
}

void sixth_task()
{
    const int n = 6;
    std::cout << "=== TASK 6 === e = 10^-" << n << " ===\n";

    // ====================== a ======================
    std::cout << "\n=== a) sin x - 2x^2 + 0.5 = 0 ===";
    std::cout << "\nLocalization: root 1 ∈ (-0.5, 0), root 2 ∈ (0.5, 1)\n";
    auto f_a = [](double x){ return sin(x) - 2*x*x + 0.5; };
    auto df_a = [](double x){ return cos(x) - 4*x; };

    dichotomy(f_a, -0.5, 0.0, n, "a1 (negative)");
    run_newton(f_a, df_a, -0.25, n, "a1");
    dichotomy(f_a, 0.5, 1.0, n, "a2 (positive)");
    run_newton(f_a, df_a, 0.75, n, "a2");

    // ====================== b ======================
    std::cout << "\n=== b) x^n = a (example n=3, a=2) ===";
    auto f_b = [](double x){ return pow(x,3) - 2; };
    auto df_b = [](double x){ return 3*pow(x,2); };
    dichotomy(f_b, 1.0, 2.0, n, "b");
    run_newton(f_b, df_b, 1.2, n, "b");

    // ====================== c ======================
    std::cout << "\n=== c) √(1−x^2) - e^x + 0.1 = 0 ===";
    std::cout << "\nLocalization: root ∈ (0, 0.5)\n";
    auto f_c = [](double x){ return sqrt(1-x*x) - exp(x) + 0.1; };
    auto df_c = [](double x){ return -x/sqrt(std::max(1e-12,1-x*x)) - exp(x); };
    dichotomy(f_c, 0.0, 0.5, n, "c");
    run_newton(f_c, df_c, 0.2, n, "c");

    // ====================== d ======================
    std::cout << "\n=== d) x^6 = 5x^3 + 2 ===";
    auto f_d = [](double x){ return pow(x,6) - 5*pow(x,3) - 2; };
    auto df_d = [](double x){ return 6*pow(x,5) - 15*pow(x,2); };
    dichotomy(f_d, 1.0, 2.0, n, "d");
    run_newton(f_d, df_d, 1.5, n, "d");

    // ====================== e ======================
    std::cout << "\n=== e) log₂x = 1/(1+x^2) ===";
    std::cout << "\nLocalization: root ∈ (1, 2)\n";
    auto f_e = [](double x){ return log2(x) - 1.0/(1+x*x); };
    auto df_e = [](double x){ return 1.0/(x*log(2)) + 2*x/pow(1+x*x,2); };
    dichotomy(f_e, 1.0, 2.0, n, "e");
    run_newton(f_e, df_e, 1.5, n, "e");

    // ====================== f ======================
    std::cout << "\n=== f) sin(x/2) = 1 ===";
    std::cout << "\nFirst three roots by modulus: -3π, π, 5π\n";
    auto f_f = [](double x){ return sin(x/2) - 1; };
    auto df_f = [](double x){ return 0.5*cos(x/2); };
    std::vector<double> roots_f = {-3*M_PI, M_PI, 5*M_PI};
    for (size_t i = 0; i < roots_f.size(); ++i) {
        double r = roots_f[i];
        dichotomy(f_f, r-0.1, r+0.1, n, "f" + std::to_string(i+1));
        run_newton(f_f, df_f, r, n, "f" + std::to_string(i+1));
    }

    // ====================== g ======================
    std::cout << "\n=== g) ln x = 1 ===";
    std::cout << "\nLocalization: root ∈ (1, 3)\n";
    auto f_g = [](double x){ return log(x) - 1; };
    auto df_g = [](double x){ return 1/x; };
    dichotomy(f_g, 1.0, 3.0, n, "g");
    run_newton(f_g, df_g, 2.0, n, "g");

    std::cout << "\n=== Research completed ===\n";
}

#endif //INC_3_HW_6_TASK_HPP
