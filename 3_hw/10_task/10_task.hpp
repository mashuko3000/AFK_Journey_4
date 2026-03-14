//
// Created by mvideo on 14/03/26.
//

#ifndef INC_3_HW_10_TASK_HPP
#define INC_3_HW_10_TASK_HPP

#include<vector>
#include<cmath>
#include<algorithm>
#include <iostream>
#include<functional>
#include <iomanip>

typedef std::vector<double> vector_t;
typedef std::vector<std::vector<double>> matrix_t;

vector_t solveGauss(matrix_t A, vector_t B)
{
    int n = B.size();
    for(int i = 0; i < n; ++i)
    {
        int pivot = i;
        for(int j = i + 1; j < n; j++)
        {
            if (std::abs(A[j][i]) > std::abs(A[pivot][i])) pivot = j;

        }
        std::swap(A[i], A[pivot]);
        std::swap(B[i], B[pivot]);

        if(std::abs(A[i][i]) < 1e-18)
        {
            throw std::runtime_error("matrix_t is singular");
        }

        for(int j = i + 1; j < n; j++)
        {
            double factor = A[j][i]/A[i][i];
            B[j] -= factor * B[i];
            for(int k = i; k < n; ++k) A[j][k] -= factor*A[i][k];
        }
    }
    vector_t x(n);
    for(int i = n - 1; i >= 0; --i)
    {
        double sum = 0;
        for(int j = i + 1; j < n; ++j) sum+=A[i][j]*x[j];
        x[i] = (B[i] - sum) / A[i][i];
    }
    return x;
}

double infNorm(
        const vector_t& a,
        const vector_t& b
        )
{
    double maxD = 0;
    for(size_t i = 0; i < a.size(); ++i) maxD = std::max(maxD, std::abs(a[i] - b[i]));
    return maxD;
}

void solveNewton(
        int dim,
        vector_t x0,
        std::function<vector_t(vector_t)> F,
        std::function<matrix_t(vector_t)> getJ,
        int n_prec
        )
{
    double eps = pow(10.0, -n_prec);
    vector_t x = x0;
    int iter = 0;

    std::cout << "\n=== TASK 10 | n = " << n_prec << " | " << dim << " variables ===\n";
    std::cout << "Iter | ";
    for(int i=1; i<=dim; ++i) std::cout << "      x" << i << "      | ";
    std::cout << "  ||DX||_inf\n" << std::string(15 * dim + 25, '-') << std::endl;

    while(iter < 150)
    {
        vector_t Fval = F(x);
        matrix_t J = getJ(x);

        vector_t negF = Fval;
        for(auto& v : negF)
        {
            v = -v;
        }

        vector_t delta = solveGauss(J, negF);
        vector_t nextX = x;
        for(int i = 0; i < dim; ++i) nextX[i] += delta[i];

        double norm = infNorm(nextX, x);

        std::cout << std::setw(4) << iter << " | ";
        for (double val : x) std::cout << std::fixed << std::setprecision(8) << std::setw(14) << val << " | ";
        std::cout << std::scientific << std::setprecision(2) << std::setw(10) << norm << std::endl;

        if(norm < eps)
        {
            std::cout << "\nConverged! Iterations: " << iter << "\nSolution: (";
            for (int i=0; i<dim; ++i) std::cout << std::fixed << std::setprecision(10) << x[i] << (i==dim-1 ? "" : ", ");
            std::cout << ")\n";
            return;
        }
        x = nextX;
        iter++;
    }
}

void tenth_task()
{
    int n = 6;

    auto Fa = [](vector_t v) -> vector_t
    {
        return
        {
            pow(v[0],3) + pow(v[0],2)*v[1] - v[0]*v[2] + 6,
            exp(v[0]) + exp(v[1]) - v[2],
            pow(v[1],2) - 2*v[0]*v[2] - 4
        };
    };
    auto Ja = [](vector_t v) -> matrix_t
    {
        return
        {
            {3*v[0]*v[0] + 2*v[0]*v[1] - v[2], v[0]*v[0], -v[0]},
            {exp(v[0]), exp(v[1]), -1},
            {-2*v[2], 2*v[1], -2*v[0]}
        };
    };
    solveNewton(3, {1.0, 1.0, 1.0}, Fa, Ja, n);

    auto Fb = [](vector_t v) -> vector_t
    {
        return
        {
            6*v[0] - 2*cos(v[1]*v[2]) - 1,
            9*v[1] + sqrt(v[0]*v[0] + sin(v[2]) + 1.06) + 0.9,
            60*v[2] + 3*exp(-v[0]*v[1]) + 10*M_PI - 3
        };
    };
    auto Jb = [](vector_t v) -> matrix_t
    {
        double root = sqrt(v[0]*v[0] + sin(v[2]) + 1.06);
        return
        {
            {6.0, 2*v[2]*sin(v[1]*v[2]), 2*v[1]*sin(v[1]*v[2])},
            {v[0]/root, 9.0, cos(v[2])/(2*root)},
            {-3*v[1]*exp(-v[0]*v[1]), -3*v[0]*exp(-v[0]*v[1]), 60.0}
        };
    };
    solveNewton(3, {0.5, -0.2, -0.52}, Fb, Jb, n);

    auto Fc = [](vector_t v) -> vector_t
    {
        return
        {
            4*v[0] - v[1] + v[2] - v[0]*v[3],
            -v[0] + 3*v[1] - 2*v[2] - v[1]*v[3],
            v[0] - 2*v[1] + 3*v[2] - v[2]*v[3],
            v[0]*v[0] + v[1]*v[1] + v[2]*v[2] - 1
        };
    };
    auto Jc = [](vector_t v) -> matrix_t
    {
        return
        {
            {4 - v[3], -1, 1, -v[0]},
            {-1, 3 - v[3], -2, -v[1]},
            {1, -2, 3 - v[3], -v[2]},
            {2*v[0], 2*v[1], 2*v[2], 0}
        };
    };
    solveNewton(4, {0.1, 0.7, 0.7, 1.1}, Fc, Jc, n);
}

#endif //INC_3_HW_10_TASK_HPP
