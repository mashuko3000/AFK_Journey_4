//
// Created by mvideo on 14/03/26.
//

#ifndef INC_3_HW_13_TASK_HPP
#define INC_3_HW_13_TASK_HPP

#include <iostream>
#include <vector>
#include <iomanip>

std::vector<double> invertSeries(
        const std::vector<double>& a,
        int n
        )
{
    if (a.empty() || a[0] == 0) return {};
    std::vector<double> b(n);
    b[0] = 1.0 / a[0];
    for (int k = 1; k < n; ++k)
    {
        double sum = 0.0;
        for (int i = 1; i <= k && i < (int)a.size(); ++i)
        {
            sum += a[i] * b[k - i];
        }
        b[k] = -b[0] * sum;
    }
    return b;
}

double factorial(int k)
{
    double res = 1.0;
    for (int i = 2; i <= k; ++i) res *= i;
    return res;
}

void thirteenth_task()
{
    std::cout << std::fixed << std::setprecision(10);

    int M = 10;
    int n = 8;

    std::vector<double> a1(M + 1);

    for (int k = 0; k <= M; ++k)
    {
        a1[k] = 1.0 / factorial(k);
    }

    auto b1 = invertSeries(a1, n);
    std::cout << "Exm. 1: f = sum_{k=0}^" << M << " x^k / k!\n";
    std::cout << "First " << n << " coeffs f^{-1}:\n";
    for (int i = 0; i < n; ++i)
    {
        std::cout << "b[" << i << "] = " << b1[i] << "\n";
    }

    std::vector<double> a2 = {-1.0, -1.0, 1.0};
    n = 6;
    auto b2 = invertSeries(a2, n);

    std::cout << "\nExm. 2: f = x^2 - x - 1\n";
    std::cout << "First " << n << " coeffs. f^{-1}:\n";
    for (int i = 0; i < n; ++i)
    {
        std::cout << "b[" << i << "] = " << b2[i] << "\n";
    }
}

#endif //INC_3_HW_13_TASK_HPP
