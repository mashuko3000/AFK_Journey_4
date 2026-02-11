#include<iostream>
#include<string>
#include<vector>
#include<fstream>

#include <ginac/ginac.h>
#include <ginac/parser.h>

using namespace GiNaC;

int main()
{
    double a;
    std::cout << "enter the base point 'a': ";
    std::cin >> a;

    int k;
    std::cout << "enter the max degree of polynomial : ";
    std::cin >> k;

    symbol x("x");
    symtab table;
    table["x"] = x;
    parser reader(table);

    std::vector<ex> f_coeffs(k + 1);
    for (int j = 0; j <= k; ++j)
    {
        std::string coeff_str;
        std::cout << "enter coefficient f_" << j << " (at (x - a)^" << j << ") : ";
        std::cin >> coeff_str;
        f_coeffs[j] = reader(coeff_str).normal();
    }

    double b;
    std::cout << "enter target point b : ";
    std::cin >> b;

    ex delta = b - a;

    for (int i = 0; i <= k; ++i)
    {
        for (int j = k - 1; j>=i; --j)
        {
            f_coeffs[j] = (f_coeffs[j] + delta * f_coeffs[j + 1]).normal();
        }
    }

    std::string bracket_name = "(x - " + std::to_string(b) + ")";
    symbol bracket(bracket_name); 

    ex f_display = 0;
    for (int i = 0; i <= k; ++i) {
        f_display += f_coeffs[i] * pow(bracket, i);
    }

    std::cout << "result : f(x) = " << f_display << std::endl;
    return 0;
}