//
// Created by mvideo on 14/03/26.
//

#ifndef INC_3_HW_9_TASK_HPP
#define INC_3_HW_9_TASK_HPP

#include <iostream>
#include <cmath>
#include <iomanip>
#include <functional>
#include <vector>
#include <string>

struct vector2D
{
    double x, y;
    double norm() const{return sqrt(x*x + y*y);}
};

vector2D solveGauss(double J[2][2], double F[2])
{
    double a = J[0][0];
    double b = J[0][1];
    double c = J[1][0];
    double d = J[1][1];

    double rhs = -F[0], rhs1 = -F[1];
    double det = a*d - b*c;

    if(std::abs(det) < 1e-15) return {0, 0};

    double dx = (rhs*d - b*rhs1) / det;
    double dy = (a*rhs1 - c*rhs) / det;
    return {dx, dy};
}

void solveNewton2D(
        std::function<vector2D(vector2D)> F,
        std::function<void(vector2D, double[2][2])> J,
        vector2D x0,
        double eps,
        std::string name
        )
{
    vector2D x = x0;
    int iter = 0;

    std::cout << "\n=== " << name << " ===\n";
    std::cout << "Initial guess: (" << x0.x << ", " << x0.y << ")\n";
    std::cout << std::setw(5) << "Iter"
         << std::setw(15) << "x"
         << std::setw(15) << "y"
         << std::setw(15) << "||F||"
         << std::setw(15) << "||ΔX||" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    while(true)
    {
        vector2D Fval = F(x);
        double Fnorm = Fval.norm();

        double Jac[2][2];
        J(x, Jac);
        double F_arr[2] = {Fval.x, Fval.y};
        vector2D delta = solveGauss(Jac, F_arr);
        double delta_norm = delta.norm();

        std::cout << std::setw(5) << iter
             << std::fixed << std::setprecision(8)
             << std::setw(15) << x.x
             << std::setw(15) << x.y
             << std::setw(15) << Fnorm
             << std::setw(15) << delta_norm << std::endl;

        if(Fnorm < eps && delta_norm < eps)
        {
            std::cout << "\nConverged in " << iter << " iterations!\n";
            std::cout << "Final solution: ("
                 << std::setprecision(10) << x.x << ", " << x.y << ")\n";
            break;
        }

        x.x += delta.x;
        x.y += delta.y;
        iter++;

        if (iter > 100) {
            std::cout << "Did not converge (max iterations reached)\n";
            break;
        }
    }
}

void ninth_task()
{
    double eps = 1e-6;

    std::cout << "=== TASK 9 ===\n";

    struct Case
    {
        double A, alpha2, beta2;
        std::string label;
        vector2D start;
    };

    std::vector<Case> cases =
    {
            {0.2, 1.0/0.6, 1.0/2.0, "i.  A=0.2, α^2=1/0.6, β^2=1/2", {0.8, 0.8}},
            {0.4, 1.0/0.8, 1.0/2.0, "ii. A=0.4, α^2=1/0.8, β^2=1/2", {0.8, 0.8}},
            {0.3, 1.0/0.2, 1.0/3.0, "iii.A=0.3, α^2=1/0.2, β^2=1/3", {0.7, 1.1}},
            {0.0, 1.0/0.6, 1.0/2.0, "iv. A=0,   α^2=1/0.6, β^2=1/2", {0.8, 0.8}}
    };

    for (const auto& c : cases)
    {
        auto F_a = [c](vector2D v) -> vector2D
        {
            double xyA = v.x * v.y + c.A;
            return
            {
                    tan(xyA) - v.x * v.x,
                    (v.x*v.x)/c.alpha2 + (v.y*v.y)/c.beta2 - 1.0
            };
        };

        auto J_a = [c](vector2D v, double J[2][2])
        {
            double xyA = v.x * v.y + c.A;
            double sec2 = 1.0 / cos(xyA) / cos(xyA);
            J[0][0] = sec2 * v.y - 2*v.x;
            J[0][1] = sec2 * v.x;
            J[1][0] = 2*v.x / c.alpha2;
            J[1][1] = 2*v.y / c.beta2;
        };

        solveNewton2D(F_a, J_a, c.start, eps, c.label);
    }

    std::cout << "\n=== TASK 9.b ===\n";
    auto F_b = [](vector2D v) -> vector2D
    {
        return
        {
                v.x*v.x + v.y*v.y - 2.0,
                exp(v.x - 1.0) + pow(v.y, 3) - 2.0
        };
    };

    auto J_b = [](vector2D v, double J[2][2])
    {
        J[0][0] = 2.0 * v.x;
        J[0][1] = 2.0 * v.y;
        J[1][0] = exp(v.x - 1.0);
        J[1][1] = 3.0 * v.y * v.y;
    };

    solveNewton2D(F_b, J_b, {1.2, 0.8}, eps, "9.b: x^2 + y^2 = 2, e^{x-1} + y^3 = 2");
}

#endif //INC_3_HW_9_TASK_HPP
