//
// Created by mvideo on 14/03/26.
//

#ifndef INC_3_HW_4_TASK_HPP
#define INC_3_HW_4_TASK_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <string>
#include <functional>

using namespace std;

double phi_a(double x, double a, double b, double c)
{
    return c + a * pow(sin(x), 2) + b * pow(cos(x), 2);
}

double phi_b(double x, double a, double b, double c)
{
    return c + a * exp(-b * x * x);
}

bool checkConvergence(const function<double(double)>& phi, double x0, double eps = 1e-8, int maxIter = 500)
{
    double x = x0;
    for (int i = 0; i < maxIter; ++i)
    {
        double x_next = phi(x);
        double diff = abs(x_next - x);

        if (diff < eps) return true;
        if (diff > 1e10 || isnan(x_next) || isinf(x_next)) return false;

        x = x_next;
    }
    return false;
}

void perform_research_a()
{
    cout << "\n" << string(60, '=') << "\n";
    cout << "RESEARCH A: phi(x) = c + a*sin^2(x) + b*cos^2(x)\n";
    cout << "Theoretical condition for global convergence: |a - b| < 1\n";
    cout << string(60, '=') << "\n";

    vector<pair<double, double>> tests =
    {
            {0.4, 0.0}, {0.9, 0.0}, {0.99, 0.0},
            {1.01, 0.0}, {1.5, 0.0}, {2.0, 0.0},
            {0.3, 0.8}, {1.2, 0.5}
    };

    for (auto [a, b] : tests)
    {
        double c = 1.0;
        auto target_phi = [a, b, c](double x)
        {
            return phi_a(x, a, b, c);
        };

        double max_der = abs(a - b);

        bool numerically_converges = true;
        for (double x0 = -100.0; x0 <= 100.0; x0 += 5.0)
        {
            if (!checkConvergence(target_phi, x0, 1e-8, 1000))
            {
                numerically_converges = false;
                break;
            }
        }

        cout << "a=" << setw(6) << a
             << "  b=" << setw(6) << b
             << "  |a-b|=" << setw(5) << fixed << setprecision(3) << max_der
             << "  | max|φ'| = " << max_der
             << "  | " << (max_der < 1.0 ? "CONVERGES глобально ✓"
                                         : "DIVERGES (или не гарантировано) ✗")
             << "  | Numerical: " << (numerically_converges ? "OK" : "FAIL") << endl;
    }
}

void perform_research_b()
{
    cout << "\n" << string(60, '=') << "\n";
    cout << "RESEARCH B: phi(x) = c + a*e^(-b*x^2)\n";
    cout << "Theoretical condition: b > 0 AND |a| * sqrt(2b/e) < 1\n";
    cout << string(60, '=') << "\n";

    struct TestCase
    {
        double a, b;
        string description;
    };

    const double E_CONST = 2.718281828;

    vector<TestCase> tests =
    {
            {1.0, 0.5,  "Condition satisfied"},
            {2.0, 0.1,  "Condition satisfied"},
            {1.5, 1.0,  "Borderline/Unstable"},
            {5.0, 1.0,  "Condition violated"},
            {1.0, -0.1, "b < 0 (Exp grows)"}
    };

    for (const auto& t : tests)
    {
        auto target_phi = [t](double x) {return phi_b(x, t.a, t.b, 0.5);};

        bool is_globally_convergent = true;
        for (double x0 = -10.0; x0 <= 10.0; x0 += 2.0)
        {
            if (!checkConvergence(target_phi, x0))
            {
                is_globally_convergent = false;
                break;
            }
        }

        double criterion = (t.b > 0) ? abs(t.a) * sqrt(2.0 * t.b / E_CONST) : -1.0;

        cout << "a=" << setw(5) << t.a << ", b=" << setw(5) << t.b
             << " | Crit.Val: " << (t.b > 0 ? to_string(criterion).substr(0, 5) : "N/A  ")
             << " | " << (is_globally_convergent ? "CONVERGES" : "DIVERGES ")
             << " | Note: " << t.description << endl;
    }
}

void forth_task()
{
    cout << fixed << setprecision(3);

    perform_research_a();
    perform_research_b();

    cout << "\nResearch study completed." << endl;
}

#endif //INC_3_HW_4_TASK_HPP
