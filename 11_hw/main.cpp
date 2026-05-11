#include <iostream>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>
#include "helpers.hpp"
template<typename T>
T headlog(int n, int p);

template<typename T>
T calculateAsymptoticUpperBound(int n, int p);

int main(int, char**){
    /*
    int n = 1000;
    int p = 10;

    std::cout << "=== n = " << n << ", p = " << p << " ===" << std::endl;

    bigint res1 = headlog<bigint>(n, p);
    std::cout << "[Метод 1] Прямой:       " << res1 << " | Операций: " << currStats.ops << std::endl;

    bigint res2 = calculateAsymptoticUpperBound<bigint>(n, p);
    std::cout << "[Метод 2] Асимптотика:  " << res2 << " | Операций: " << currStats.ops << std::endl;

    bigint res3 = calculateExact<bigint>(n, p);
    std::cout << "[Метод 3] Точный (холд):" << res3 << " | Операций: " << currStats.ops << std::endl;

    bigint res3_hot = calculateExact<bigint>(n, p);
    std::cout << "[Метод 3] Точный (горяч):" << res3_hot << " | Операций: " << currStats.ops << std::endl;

    return 0;*/

    std::ofstream csv("results.csv");
    if (!csv.is_open())
    {
        std::cerr << "Error: File results.csv wasn't created!" << std::endl;
        return 1;
    }

    csv << "Method,n,p,Operations\n";

    std::cout << "Start gathering data..." << std::endl;

    int fixed_p = 10;
    std::cout << "Start Experiment num 1, p is fixed" << fixed_p << ", n is changing" << std::endl;

    for (int n = 10; n <= 100000; n *= 10)
    {
        headlog<bigint>(n, fixed_p);
        csv << "Direct," << n << "," << fixed_p << "," << currStats.ops << "\n";

        calculateAsymptoticUpperBound<bigint>(n, fixed_p);
        csv << "Asymptotic," << n << "," << fixed_p << "," << currStats.ops << "\n";

        calculateExact<bigint>(n, fixed_p);
        csv << "Exact," << n << "," << fixed_p << "," << currStats.ops << "\n";
    }


    int fixed_n = 1000;
    std::cout << "Start Experiment num 2, n is fixed = " << fixed_n << ", p is changing" << std::endl;

    for (int p = 2; p <= 20; p += 2)
    {
        headlog<bigint>(fixed_n, p);
        csv << "Direct," << fixed_n << "," << p << "," << currStats.ops << "\n";

        calculateAsymptoticUpperBound<bigint>(fixed_n, p);
        csv << "Asymptotic," << fixed_n << "," << p << "," << currStats.ops << "\n";

        calculateExact<bigint>(fixed_n, p);
        csv << "Exact," << fixed_n << "," << p << "," << currStats.ops << "\n";
    }

    csv.close();
    std::cout << "All good" << std::endl;

    std::cout << "Start python script" << std::endl;
    int result = system("python3 plotter.py results.csv");
    if (result == 0)
    {
        std::cout << "Plotters plot_n.png и plot_p.png was created succesfully." << std::endl;
    }
    else
    {
        std::cout << "Error : Reason : Python scrypt" << std::endl;
    }

    return 0;
}


template<typename T>
T headlog(int n, int p)
{
    currStats.reset();

    OpCounter<T> sum(0);
    for(int k = 1; k <= n; ++k)
    {
        OpCounter<T> currentK(k);
        sum += currentK.fastPow(p);
    }
    return sum.raw();
}

template<typename T>
T calculateAsymptoticUpperBound(int n, int p)
{
    currStats.reset();
    
    OpCounter<cpp_rational> N(n);
    OpCounter<cpp_rational> P(p);

    OpCounter<cpp_rational> main_term = N.fastPow(p + 1) / (p + 1);
    OpCounter<cpp_rational> rho_term = N.fastPow(p) / 2;

    OpCounter<cpp_rational> sigma_upper_bound = (P * N.fastPow(p - 1)) / 8;
    OpCounter<cpp_rational> total = main_term + rho_term + sigma_upper_bound;

    return numerator(total.raw()) / denominator(total.raw());
}


