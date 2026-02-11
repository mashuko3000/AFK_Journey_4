#include<iostream>
#include<string>
#include<fstream>
#include<exception>
#include <cmath>

#include <ginac/ginac.h>
#include <ginac/parser.h>

using namespace GiNaC;

struct leading_term{
    double coeff;
    int degree;
};

leading_term infinity(const ex& f, const ex& s, int power, const symbol& x)
{
    if(f.is_zero() || s.is_zero()) return {0.0, 0};

    int deg_f = f.degree(x);
    int deg_s = s.degree(x);

    if (deg_f == 0 && deg_s == 0)
    {
        double val = ex_to<numeric>(f).to_double();
        return {std::pow(val, power), 0};
    }
    
    double lc_f = ex_to<numeric>(f.coeff(x, deg_f)).to_double();
    double lc_s = ex_to<numeric>(s.coeff(x, deg_s)).to_double();

    double lc_comp = lc_f * std::pow(lc_s, deg_f);

    double final_coeff = std::pow(lc_comp, power);
    int final_deg = power * deg_f * deg_s;

    return {final_coeff, final_deg};
}

std::vector<double> horner_scheme(const ex& f, const symbol& x, double A)
{
    int deg = f.degree(x);
    std::vector<double> coeffs;
    for (int i = 0; i<=deg; ++i)
    {
        coeffs.push_back(ex_to<numeric>(f.coeff(x, i)).to_double());
    }
    int n = coeffs.size();
    std::vector<double> result(n);

    for(int i = 0; i<n; ++i)
    {
        for(int j = n - 1; j>i; --j)
        {
            coeffs[j - 1] += coeffs[j] * A;
        }
        result[i] = coeffs[i];
    }
    return result;
}

leading_term get_point_term(const ex& f, const ex& s, int power, const symbol&x, double A)
{
    std::vector<double> s_shifted = horner_scheme(s, x, A);
    double s_at_a = s_shifted[0];

    std::vector<double> f_shifted = horner_scheme(f, x, s_at_a);

    int first_coef = 0;
    while(first_coef < f_shifted.size() && std::abs(f_shifted[first_coef]) < 1e-12) first_coef++;

    if (first_coef == f_shifted.size()) return {0.0, 0};

    double res_coeff;
    int res_pwr;

    if (first_coef == 0)
    {
        res_coeff = f_shifted[0];
        res_pwr = 0;
    }
    else
    {
        int f_s = 1;
        while(f_s < s_shifted.size() && std::abs(s_shifted[f_s]) < 1e-12) f_s++;

        if (f_s == s_shifted.size()) return {0.0, 0}; 

        res_coeff = f_shifted[first_coef] * std::pow(s_shifted[f_s], first_coef);
        res_pwr = first_coef * f_s;
    }
    
    return {std::pow(res_coeff, power), res_pwr * power};
}

int main()
{
    symbol x("x");
    symtab table;
    table["x"] = x;
    parser reader(table);

    try
    {
        std::string filename;
        std::cout << "Enter filename with all polynomials: ";
        std::cin >> filename;

        std::ifstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);

        std::string line;

        getline(file, line);
        ex f1 = reader(line);

        getline(file, line);
        ex s1 = reader(line);

        getline(file, line);
        ex f2 = reader(line);

        getline(file, line);
        ex s2 = reader(line);

        file.close();

        std::cout << "All polynomials loaded successfully!" << std::endl;

        int k, l;

        std::cout << "Enter power k for numerator: ";
        std::cin >> k;
        std::cout << "Enter power l for denominator: ";
        std::cin >> l;

        leading_term num = infinity(f1, s1, k, x);
        leading_term den = infinity(f2, s2, l, x);

        std::cout << "Numerator:   " << num.coeff << " * x^" << num.degree << std::endl;
        std::cout << "Denominator: " << den.coeff << " * x^" << den.degree << std::endl;

        if (num.degree > den.degree) {
            if (num.coeff * den.coeff > 0) std::cout << "Limit x->+inf: +inf" << std::endl;
            else std::cout << "Limit x->+inf: -inf" << std::endl;
        } 
        else if (num.degree < den.degree) {
            std::cout << "Limit x->+inf: 0" << std::endl;
        } 
        else {
            std::cout << "Limit x->+inf: " << num.coeff / den.coeff << std::endl;
        }

        double A;
        std::cout << "\n Enter point A: ";
        std::cin >> A;

        leading_term num_pt = get_point_term(f1, s1, k, x, A);
        leading_term den_pt = get_point_term(f2, s2, l, x, A);

        std::cout << "Analysis near x = " << A << ":" << std::endl;
        std::cout << "Numerator   ~ " << num_pt.coeff << " * (x-A)^" << num_pt.degree << std::endl;
        std::cout << "Denominator ~ " << den_pt.coeff << " * (x-A)^" << den_pt.degree << std::endl;

        if (num_pt.degree > den_pt.degree)
        {
            std::cout << "Result: 0" << std::endl;
        } 
        else if (num_pt.degree < den_pt.degree)
        {
            std::cout << "Result: Infinity" << std::endl;
        }
        else
        {
            if (std::abs(den_pt.coeff) < 1e-15) std::cout << "Result: Undefined" << std::endl;
            else std::cout << "Result: " << num_pt.coeff / den_pt.coeff << std::endl;
        }
    }catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}