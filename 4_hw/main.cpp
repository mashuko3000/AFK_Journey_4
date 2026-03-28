#include <iostream>
#include"inc/Polynimial.hpp"
#include<complex>
#include"inc/ZMod.hpp"
/*
int main()
{
    std::vector<std::string> vars = {"x", "y"};

    std::cout << "=== Field R (double) ===\n";
    Polynomial<double> f_R(vars);
    Polynomial<double> i_R(vars);

    f_R.addMonomial({2, 0}, 3.5);
    f_R.addMonomial({1, 1}, 2.0);

    i_R.addMonomial({2, 0}, 4.0);
    i_R.addMonomial({3, 5}, 8.0);

    std::cout << "f_R = " << f_R << "\n";

    f_R.addMonomial({0, 1}, 5.0);

    auto [h_R, g_R] = f_R.decomposeHomogeneous(2);
    std::cout << "After adding 5y = " << f_R << "\n";
    std::cout << " h (pow 2) = " << h_R << "\n";
    std::cout << "Reminder g (f = h + g) = " << g_R << "\n";

    Polynomial<double> d_R(vars);
    d_R.addMonomial({2, 0}, 3.5);
    d_R.addMonomial({0, 2}, 2.0);

    int degree = d_R.degreeOfHomogeneity();

    std::cout << d_R << std::endl;
    std::cout << "Degree is " << degree << std::endl;

    degree = f_R.degreeOfHomogeneity();
    std::cout << f_R << std::endl;
    std::cout << "Degree is " << degree << std::endl;

    std::cout << "Result "<< (f_R - i_R) << std::endl;

    std::cout << "\n=== Field C (complex) ===\n";
    Polynomial<std::complex<double>> f_C(vars);
    f_C.addMonomial({1, 0}, std::complex<double>(0, 1));
    f_C.addMonomial({0, 1}, std::complex<double>(2, -1));
    std::cout << "f_C = " << f_C << "\n";

    Polynomial<std::complex<double>> f_C2(vars);
    f_C2.addMonomial({1, 0}, std::complex<double>(0, 1));
    std::cout << "f_C * i*x = " << (f_C * f_C2) << "\n";

    std::cout << "\n=== Ring Z_5 ===\n";
    Polynomial<ZMod<5>> f_Z(vars);
    f_Z.addMonomial({2, 0}, ZMod<5>(3));
    f_Z.addMonomial({0, 2}, ZMod<5>(4));
    std::cout << "f_Z = " << f_Z << "\n";

    Polynomial<ZMod<5>> g_Z(vars);
    g_Z.addMonomial({2, 0}, ZMod<5>(2));

    Polynomial<ZMod<5>> sum_Z = f_Z + g_Z;
    std::cout << "f_Z + (2x^2) = " << sum_Z << "  <-- (3+2)x^2 become 0 in Z_5\n";

    std::vector<ZMod<5>> point_Z = {ZMod<5>(2), ZMod<5>(3)};
    std::cout << "f_Z(2, 3) в Z_5 = " << f_Z.evaluate(point_Z) << "\n";

    return 0;
}*/