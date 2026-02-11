#include<iostream>
#include<string>
#include<fstream>
#include<exception>

#include <ginac/ginac.h>
#include <ginac/parser.h>

using namespace GiNaC;

ex read_function(const std::string& filename, const symbol& x, parser& reader);

std::vector<ex> get_coeffs(const ex& poly, const symbol& x, const ex& A)
{
    int deg = poly.degree(x);
    std::vector<ex> c(deg +1);
    for(int i = 0; i <= deg; ++i)
    {
        c[i] = poly.coeff(x, i);
    }
    
    for (int j = 0; j <= deg; ++j)
    {
        for (int i = c.size() - 2; i >= 0; --i) {
            c[i] = c[i] + c[i+1] * A;
        }
        c.push_back(c[0]);
        c.erase(c.begin());
    }
    return c;
}

int main()
{
    symbol x("x");
    symtab table;
    table["x"] = x;
    parser reader(table);

    std::string f_filename;
    std::cout << "Enter name of file for function f : " << std::endl;
    std::cin >> f_filename;

    std::string g_filename;
    std::cout << "Enter name of file for function g : " << std::endl;
    std::cin >> g_filename;

    try
    {
        ex f = read_function(f_filename, x, reader);
        ex g = read_function(g_filename, x, reader);

        std::cout << "R(x) = (" << f << ") / (" << g << ")" << std::endl;

        double a_val;

        std::cout << "Enter point A for limit: ";
        std::cin >> a_val;

        ex a = a_val;


        int deg_f = f.degree(x);
        int deg_g = g.degree(x);

        std::cout << "Limit x -> oo: ";
        if (deg_f > deg_g) std::cout<< "oo" << std::endl;
        else if(deg_f < deg_g) std::cout << "0" << std::endl;
        else std::cout << (f.coeff(x, deg_f) / g.coeff(x, deg_g)).normal() << std::endl;


        std::vector<ex> c_f = get_coeffs(f, x, a);
        std::vector<ex> c_g = get_coeffs(g, x, a);

        int k = 0; 
        while (k < c_f.size() && c_f[k].is_zero()) k++;
        
        int m = 0; 
        while (m < c_g.size() && c_g[m].is_zero()) m++;

        std::cout << "Limit x -> " << a_val << ": ";
        if (k < m)
        {
            std::cout << "oo" << std::endl;
        }
        if (k > m)
        {
            std::cout << "0" << std::endl;
        }
        else std::cout << (c_f[k] / c_g[m]) << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}

ex read_function(const std::string& filename, const symbol& x, parser& reader)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    getline(file, line);
    file.close();

    std::cout << "String from file [" << line << "]" << std::endl;
    return reader(line);
}
