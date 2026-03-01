//
// Created by mvideo on 27/02/26.
//
#include"task8_1.hpp"

complex seven_operation_division_mult(
        const double a0,
        const double a1,
        const double b0,
        const double b1,
        int& count
        )
{
    count = 0;

    double b0_sq = b0 * b0; count++;
    double b1_sq = b1 * b1; count++;
    double denom = b0_sq + b1_sq;
    double p = a0 * b0; count++;
    double q = a1 * b1; count++;
    double r = (a0 + a1) * (b0 - b1); count++;
    double re_num = p + q;
    double im_num = r - p + q;
    double re = re_num / denom; count++;
    double im = im_num / denom; count++;

    return {re, im};
}

complex six_operation_division_mult(
        const double a0,
        const double a1,
        const double b0,
        const double b1,
        int& count
)
{
    count = 0;
    double re, im;

    if (std::abs(b0) >= std::abs(b1)) {
        double r = b1 / b0; count++;
        double den = b0 + r * b1; count++;
        re = (a0 + a1 * r) / den; count += 2;
        im = (a1 - a0 * r) / den; count += 2;
    } else {
        double r = b0 / b1; count++;
        double den = b1 + r * b0; count++;
        re = (a1 + a0 * r) / den; count += 2;
        im = (a1 * r - a0) / den; count += 2;
    }

    return {re, im};
}

complex div_standard(
        const double a0,
        const double a1,
        const double b0,
        const double b1,
        int& count
        )
{
    double denom = b0 * b0 + b1 * b1; count += 2;
    double re = (a0 * b0 + a1 * b1) / denom; count += 3;
    double im = (a1 * b0 - a0 * b1) / denom; count += 3;
    return {re, im};
}

void print_row(std::string name, complex res, int ops) {
    std::cout << std::left << std::setw(20) << name
              << "| Re: " << std::setw(10) << res.re
              << "| Im: " << std::setw(10) << res.im
              << "| Ops: " << ops << std::endl;
}

void task_8_1(
        double a0,
        double a1,
        double b0,
        double b1
        )
{
    int count_theory = 0;
    int count_7 = 0;
    int count_6 = 0;

    complex res_7;
    complex res_6;
    complex theory;



    res_7 = seven_operation_division_mult(a0, a1, b0, b1, count_7);
    res_6 = six_operation_division_mult(a0, a1, b0, b1, count_6);
    theory = div_standard(a0, a1, b0, b1, count_theory);

    std::cout << std::string(60, '-') << std::endl;
    print_row("Standard", theory, count_theory);
    print_row("7-Op (Your Logic)", res_7, count_7);
    print_row("6-Op (Smith)", res_6, count_6);
    std::cout << std::string(60, '-') << std::endl;
}