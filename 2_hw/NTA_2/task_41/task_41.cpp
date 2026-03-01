#include "task_41.hpp"

complex three_multiplication_product(
        const double a,
        const double b,
        const double c,
        const double d,
        int& count
)
{
    count = 0;

    double a_plus_b = a + b;
    double c_plus_d = c + d;

    double p = a * c; count++;
    double q = b * d; count++;
    double r = a_plus_b * c_plus_d; count++;

    double re = p - q;
    double im = r - p - q;

    return {re, im};
}

complex standard_product(
        const double a0,
        const double a1,
        const double b0,
        const double b1,
        int& count
)
{
    count = 0;

    double re = a0 * b0 - a1 * b1; count += 2;
    double im = a0 * b1 + a1 * b0; count += 2;

    return {re, im};
}

void print(std::string name, complex res, int ops) {
    std::cout << std::left << std::setw(24) << name
              << "| Re: " << std::setw(12) << res.re
              << "| Im: " << std::setw(12) << res.im
              << "| Ops: " << ops << std::endl;
}

void task_41(
        double a0, double a1,
        double b0, double b1
)
{
    int count_std = 0;
    int count_3m  = 0;

    complex res_std = standard_product (a0, a1, b0, b1, count_std);
    complex res_3m  = three_multiplication_product(a0, a1, b0, b1, count_3m);

    std::cout << std::string(70, '-') << "\n";
    print("Standard (4 mul)",   res_std, count_std);
    print("3 mul + 5 add/sub",  res_3m,  count_3m);
    std::cout << std::string(70, '-') << "\n";
}