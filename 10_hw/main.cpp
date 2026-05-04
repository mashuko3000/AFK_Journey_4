#include "bigfloat.hpp"
#include <iostream>
#include <fstream>

int main() {
    std::ofstream csv("complexity.csv");
    csv << "Digits,E_Operations,PI_Operations\n";
    
    for (int digits = 1; digits <= 100; digits += 1) {
        bigfloat eps = bigfloat(1) / bigfloat(10).pow(digits);
        bigfloat::reset_stats();
        bigfloat e_val = bigfloat::E(eps, bigfloat(1));
        long long e_ops = bigfloat::get_operations();
        bigfloat::reset_stats();
        bigfloat pi_val = bigfloat::PI(eps);
        long long pi_ops = bigfloat::get_operations();
        csv << digits << "," << e_ops << "," << pi_ops << "\n";
    }

    csv.close();
    return 0;
}