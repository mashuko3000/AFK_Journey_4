#include<vector>
#include<stdexcept>
#include<cstdint>
#include<iostream>
#include<fstream>
#include<string>
#include<complex>

#include <cmath>
#include <ginac/ginac.h>

using namespace GiNaC;

//g++ knut.cpp -o 1 -lginac -lcln

uint32_t short_division(
    const std::vector<uint32_t>& divisible, 
    uint32_t divider, 
    std::vector<uint32_t>& quotient, 
    uint64_t base);

void task_16();
void task_1();
void task_2();
void task_3();
void task_4();
void task_8();

int main()
{
    try
    {
        //task_16();
        //task_1();
        //task_2();
        //task_3();
        //task_4();
        task_8();
    }
    catch(const std::exception& e)
    {
        std::cerr << "\nERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

uint32_t short_division(
    const std::vector<uint32_t>& divisible, 
    uint32_t divider, 
    std::vector<uint32_t>& quotient, 
    uint64_t base)
{
    if(divider == 0) throw std::invalid_argument("Division by zero");
    if(divider >= base) throw std::invalid_argument("Divider must be less then base");

    int size = divisible.size();
    uint64_t remainder = 0;

    quotient.assign(size, 0);
    
    for(int j = size - 1; j >= 0; --j)
    {
        if(divisible[j] >= base) throw std::domain_error("Digit exceeds base");

        uint64_t intermediate = (remainder * base) + divisible[j];

        quotient[j] = static_cast<uint32_t>(intermediate / divider);

        remainder = intermediate % divider;
    }

    return static_cast<uint32_t>(remainder);
}

void task_16()
{
    uint64_t base = 0;
    uint32_t divider = 0;
    int n = 0;

    std::cout << "Enter the number base : ";
    if(!(std::cin >> base)) throw std::runtime_error("Invalid input for base.");

    std::cout << "\nEnter the amount of digits(n) : ";
    if(!(std::cin >> n) || n <= 0) throw std::runtime_error("Invalid amount for digits.");

    std::vector<uint32_t> divisible(n);
    std::cout << "Enter the digits from LEAST significant (u0) to MOST significant (un-1):" << std::endl;

    for(int i = 0; i < n; ++i)
    {
        std::cout << "divisible[" << i << "] : ";
        if(!(std::cin >> divisible[i])) throw std::runtime_error("Invalid digit input.");

        if(divisible[i] >= base) throw std::domain_error("Digit exceeds base");
    }

    std::cout << "Enter the divider : ";
    if(!(std::cin >> divider)) throw std::runtime_error("Invalid input for divider.");

    std::vector<uint32_t> quotient;
    uint32_t remainder = short_division(divisible, divider, quotient, base);

    std::cout << "\n==========Results==========" << std::endl;
    std::cout << "Quotient (MSB to LSB) : " << std::endl;
    bool first_digit = true;
    for (int i = n - 1; i >= 0; --i)
    {
        if (quotient[i] == 0 && first_digit && i != 0) continue;
        std::cout << quotient[i] << " ";
        first_digit = false; 
    }
    std::cout<<"\nRemainder: " << remainder << std::endl;

}

double evaluate_odd_polynomial(const std::vector<double> coeffs, double x)
{
    if(coeffs.empty()) throw std::invalid_argument("No coeffs");

    double y = x * x;
    double result = 0;

    for (int i = coeffs.size() - 1; i >= 0; --i)
    {
        result = result * y + coeffs[i];
    }

    return result * x;
}
void process_polynomial(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file) throw std::runtime_error("Cannot open the file");

    symbol x("x");
    symtab table;
    table["x"] = x;
    parser reader(table);

    std::string line;
    int line_num = 1;
    while(getline(file, line))
    {
        if(line.empty()) continue;
        ex poly = reader(line);

        std::cout << "\n------------------------------------------" << std::endl;
        std::cout << "Polynomial #" << line_num << ": " << poly << std::endl;

        bool is_odd = true;
        
        std::vector<double> coeffs;
        int max_deg = ex_to<numeric>(poly.degree(x)).to_int();

        for(int deg = 1; deg <= max_deg; deg += 2)
        {
            double c = ex_to<numeric>(poly.coeff(x, deg).evalf()).to_double();
            coeffs.push_back(c);
        }

        double x_val;
        std::cout << "Enter the value of x to evaluate: ";
        if (!(std::cin >> x_val)) 
        {
            std::cout << "Invalid input. Skipping..." << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        double res = evaluate_odd_polynomial(coeffs, x_val);
        std::cout << "Line " << line_num << "\n" << line << "\nResult : (x=" << x_val << "): " << res << "\n" << std::endl;

        line_num++;
    }
    file.close();
}

void task_1()
{
    process_polynomial("test.txt");
}

std::vector<double> algo_H(const std::vector<double>& u, double x)
{
    int n = static_cast<int>(u.size()) - 1;

    std::vector<double> v = u;

    for(int i = 0; i < n; ++i)
    {
        for (int j = n-1; j >= i; --j)
        {
            v[j] = v[j] + x * v[j +1];
        }
    }

    return v;
}

std::vector<double> algo_G(const std::vector<double>& u, double x)
{
    int n = static_cast<int>(u.size()) - 1;
    std::vector<double> v(n + 1, 0.0);

    for(int i = n; i >= 0; --i)
    {
        v[i] = u[i];
        for (int j = i; j <= n - 1; ++j)
        {
            v[j] = v[j] + x * v[j + 1];
        }
    }

    return v;
}

void print_coeffs(const std::string& label, const std::vector<double>& v) {
    std::cout << label << " coefficients: ";
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << "x" << i << " = " << v[i] << (i == v.size() - 1 ? "" : ", ");
    }
    std::cout << std::endl;
}

void task_2()
{
    symbol x("x");
    symtab table;
    table["x"] = x;
    parser reader(table);

    std::string poly_line;
    double g;

    std::cout << "Enter polynomial f(x): ";
    getline(std::cin, poly_line);
    std::cout << "Enter shift value x0: ";
    std::cin >> g;

    ex poly = reader(poly_line);

    int n = ex_to<numeric>(poly.degree(x)).to_int();

    std::vector<double> coeffs(n+1);
    for(int i = 0; i <= n; ++i)
    {
        coeffs[i] = ex_to<numeric>(poly.coeff(x, i).evalf()).to_double();
    }

    std::vector<double> res_H = algo_H(coeffs, g);
    std::vector<double> res_G = algo_G(coeffs, g);

    std::cout << "\n====== Calculation Results ======" << std::endl;
    print_coeffs("Algorithm H", res_H);
    print_coeffs("Algorithm G", res_G);
}

double evaluate_horner(
    const std::vector<std::vector<double>>& f, 
    int n, 
    double x_val, 
    double y_val)
{
    double result = 0;
    for (int i = n; i >= 0; --i)
    {
        double y_poly_val = 0.0;
        int max_j = n - i;
        for(int j = max_j; j >= 0; --j)
        {
            y_poly_val = y_poly_val * y_val + f[i][j];
        }
        if(i == n) result = y_poly_val;
        else result = result * x_val + y_poly_val;
    }
    return result;
}

void task_3()
{
    symbol x("x"), y("y");
    symtab table;
    table["x"] = x;
    table["y"] = y;
    parser reader(table);

    std::string line;
    std::cout << "Enter polynomial f(x): ";
    getline(std::cin, line);
    
    ex poly = reader(line);
/*
    int deg_x = ex_to<numeric>(poly.degree(x)).to_int();
    int deg_y = ex_to<numeric>(poly.degree(y)).to_int();

    int n = std::max(deg_x, deg_y);
    std::vector<std::vector<double>> f(n + 1, std::vector<double>(n+1, 0.0));

    for(int i = 0; i <= deg_x; ++i)
    {
        ex x_coef = poly.coeff(x, i);
        for (int j = 0; j <= deg_y; ++j)
        {
            if (i + j <= n)
            {
                f[i][j] = ex_to<numeric>(x_coef.coeff(y, j).evalf()).to_double();
            }
        }
    }
*/

    int n = 0;

    if(is_a<add>(poly))
    {
        for(const auto& term : poly)
        {
            int term_deg = term.degree(x) + term.degree(y);
            if (term_deg > n) n = term_deg;
        }
    }
    else
    {
        n = poly.degree(x) + poly.degree(y);
    }

    int expected_coeffs = (n + 1) * (n + 2) / 2;
    std::cout << "Total degree (n): " << n << std::endl;
    std::cout << "Max possible coefficients: " << expected_coeffs << std::endl;

    std::vector<std::vector<double>> f(n + 1, std::vector<double>(n+1, 0.0));

    for(int i = 0; i <= n; ++i)
    {
        ex x_coef = poly.coeff(x, i);
        for (int j = 0; j <= n - i; ++j)
        {
            f[i][j] = ex_to<numeric>(x_coef.coeff(y, j).evalf()).to_double();
        }
    }

    double xf, yf;

    std::cout << "Enter x and y: ";
    std::cin >> xf >> yf;

    double res = evaluate_horner(f, n, xf, yf);

    std::cout << "Result u(" << xf << ", " << yf << ") = " << res << std::endl;

    int ops = (n * (n + 1)) / 2 + n;
    std::cout << "Complexity: " << ops << " multiplications and " << ops << " additions." << std::endl;
}

struct op_count
{
    int mult = 0;
    int add_sub = 0;
    void reset() {mult = 0; add_sub = 0;}
};

std::complex<double> horner_complex(
    const std::vector<std::complex<double>>& f,
    std::complex<double> z,
    op_count & c
)
{
    int n = (int)f.size() - 1;
    std::complex<double>res = f[n];

    for(int i = n - 1; i >= 0; --i)
    {
        //(a+bi)*(c+di) = (ac-bd) + i(ad+bc)
        double ac = res.real() * z.real();
        double bd = res.imag() * z.imag();
        double ad = res.real() * z.imag();
        double bc = res.imag() * z.real();
        c.mult += 4;
        c.add_sub += 1;
        c.add_sub += 1;

        res = std::complex<double>((ac - bd) + f[i].real(), (ad + bc) + f[i].imag());
        c.add_sub += 2;
    }
    return res;
}

std::complex<double> scheme(
    const std::vector<std::complex<double>>& f,
    std::complex<double> z,
    op_count & c
)
{
    int n = (int)f.size() - 1;
    double x = z.real();
    double y = z.imag();

    double r = x + x;
    c.add_sub ++;

    double s = x * x + y * y;
    c.mult += 2;
    c.add_sub ++;

    std::complex<double> a = f[n];
    std::complex<double> b = f[n - 1];

    for(int j = 2; j <= n; ++j)
    {
        std::complex<double> a_prev = a;
        
        double a_re = b.real() + r * a_prev.real();
        double a_im = b.imag() + r * a_prev.imag();

        c.mult += 2;
        c.add_sub += 2;

        double b_re = f[n-j].real() - s * a_prev.real();
        double b_im = f[n-j].imag() - s * a_prev.imag();

        c.mult += 2;
        c.add_sub += 2;

        a = {a_re, a_im};
        b = {b_re, b_im};
    }
    double re = (a.real() * x - a.imag() * y) + b.real();
    double im = (a.real() * y + a.imag() * x + b.imag());
    c.mult += 4; 
    c.add_sub += 4;

    return {re, im};
}

void analyze(int n) {
    std::vector<std::complex<double>> f(n + 1, {1.5, 2.5});
    std::complex<double> z{1.1, 0.9};
    op_count c_h, c_s;

    std::complex<double> r_h = horner_complex(f, z, c_h);
    std::complex<double> r_s = scheme(f, z, c_s);

    std::cout << "Analysis for n = " << n << " (Complex coefficients)" << std::endl;
    std::cout << "Method         | Mults | Adds/Subs | Result" << std::endl;
    std::cout << "---------------|-------|-----------|-----------------" << std::endl;
    std::cout << "Horner (Sch 2) | " << c_h.mult << "    | " << c_h.add_sub << "        | " << r_h << std::endl;
    std::cout << "Scheme 3       | " << c_s.mult << "    | " << c_s.add_sub << "        | " << r_s << std::endl;
    
    std::cout << "\nTheoretical check (Scheme 3):" << std::endl;
    std::cout << "Expected Mults (4n+2): " << 4*n + 2 << " vs Real: " << c_s.mult << std::endl;
    std::cout << "Expected Adds  (4n+5): " << 4*n + 5 << " vs Real: " << c_s.add_sub << std::endl;
}

void task_4()
{
    analyze(5);
}

void task_8() {
    std::vector<double> u = {1.0, 2.0, 3.0, 4.0}; 
    double x = 5.0;
    int n = u.size() - 1;

    int mults = 0;
    int adds_subs = 0;

    double res = u[n];
    for (int i = n - 1; i >= 1; --i)
    {
        double term = (x - i); 
        adds_subs++;
        
        res = res * term + u[i];
        mults++;
        adds_subs++;
    }
    
    res = res * x + u[0];
    mults++;
    adds_subs++;

    std::cout << "Result: " << res << std::endl;
    std::cout << "Operations: mults=" << mults << ", adds/subs=" << adds_subs << std::endl;
    std::cout << "Theory check: mults=" << n << ", adds=" << 2*n-1 << std::endl;
}