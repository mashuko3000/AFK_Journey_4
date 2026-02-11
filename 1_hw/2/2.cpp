#include<iostream>
#include<string>
#include<vector>
#include<fstream>

#include <ginac/ginac.h>
#include <ginac/parser.h>

using namespace GiNaC;

int main()
{
    std::string f_filename;
    std::cout << "Enter name of file for function f : " << std::endl;
    std::cin >> f_filename;

    std::ifstream file(f_filename);
    if (!file.is_open())
    {
        std::cout << "Cannot open the file for function f(x)" << std::endl;
        return 1;
    }

    std::string line;
    getline(file, line);
    file.close();

    std::cout << "String from f file [" << line << "]" << std::endl;

    symbol x("x");
    symtab table;
    table["x"] = x;
    parser reader(table);

    ex f;

    try
    {
        f = reader(line);
        std::cout << "All good" << std::endl;
        std::cout << "f(x) = " << f << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << "Parsing fail: " << e.what() << std::endl;
        return 1;
    }

    int f_deg = f.degree(x);


    double a_val;
    std::cout << "Enter real number 'a' : " << std::endl;
    std::cin >> a_val;

    ex a = a_val;

    std::vector<ex> coeffs(f_deg + 1);
    for(int i = 0; i <= f_deg; ++i)
    {
        coeffs[i] = f.coeff(x, i);
    }

    std::vector<ex> A(f_deg + 1);

    for (int i = 0; i <= f_deg; ++i)
    {
        for (int j = f_deg - 1; j >= i; --j)
        {
            coeffs[j] = (coeffs[j] + a * coeffs[j + 1]).normal();
        }
        A[i] = coeffs[i];
    }

    ex f_taylor = 0;
    for(int k = 0; k <= f_deg; ++k)
    {
        if(!A[k].is_zero())
        {
            f_taylor += A[k] * pow(x - a, k);
        }
    }

    symbol y(" (x - " + std::to_string(a_val) + ") ");

    ex final_display = 0;
    for(int k = 0; k <= f_deg; ++k)
    {
        if(!A[k].is_zero())
        {
            final_display += A[k] * pow(y, k);
        }
    }

    std::string bracket_name = "(x - " + std::to_string(a_val) + ")";
    symbol bracket(bracket_name); 

    ex f_display = 0;
    for (int k = 0; k <= f_deg; ++k) {
        if (!A[k].is_zero()) {
            f_display += A[k] * pow(bracket, k);
        }
    }

    std::cout << "f(x) representation by powers of (x - " << a_val << "):" << std::endl;
    std::cout << f_display << std::endl;

    //std::cout << "f(x) representation by powers of (x - " << a << "): " << std::endl;
    //std::cout << final_display << std::endl;
    return 0;
}