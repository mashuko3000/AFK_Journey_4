#include "2_task.hpp"

double theo_p_fixed(int i, int N)
{
    return 1.0/N;
}

double theo_p_intersection(int i, int j, int N)
{
    if(i == j) return 1.0/N;
    return 1.0/(static_cast<double>(N)*(N-1));
}

double theo_p_union(int i, int j, int N)
{
    if (i == j) return theo_p_fixed(i, N);
    return theo_p_fixed(i, N) + theo_p_fixed(j, N) - theo_p_intersection(i, j, N);
}

void print_favorable_logic(int N, int i, int j)
{
    std::cout << "  Favorable outcomes for ";

    if (i == j)
    {
        std::cout << "A_" << i << " (fixed point at position " << i << "):\n";
        std::cout << "    [ ";
        for (int k = 1; k <= N; ++k)
        {
            std::cout << (k == i ? std::to_string(k) : "?");
            std::cout << (k < N ? " " : "");
        }
        std::cout << " ]\n";
        std::cout << "    (number " << i << " is at position " << i << ", other positions — any permutation of remaining numbers)\n";
    }
    else
    {
        std::cout << "A_" << i << " ∪ A_" << j << " (at least one fixed point):\n";

        std::cout << "    Type 1 — both fixed:\n";
        std::cout << "      [ ";
        for (int k = 1; k <= N; ++k)
        {
            if (k == i) std::cout << i;
            else if (k == j) std::cout << j;
            else std::cout << "?";
            std::cout << (k < N ? " " : "");
        }
        std::cout << " ]\n";
        std::cout << "(both " << i << " and " << j << " are in their natural positions)\n\n";

        std::cout << "Type 2 — only A_" << i << ":\n";
        std::cout << "      [ ";
        for (int k = 1; k <= N; ++k)
        {
            if (k == i) std::cout << i;
            else if (k == j) std::cout << "X";
            else std::cout << "?";
            std::cout << (k < N ? " " : "");
        }
        std::cout << " ]\n";
        std::cout << "      (" << i << " is fixed, but " << j << " is NOT at position " << j << ")\n\n";

        std::cout << "    Type 3 — only A_" << j << ":\n";
        std::cout << "      [ ";
        for (int k = 1; k <= N; ++k) {
            if (k == i)      std::cout << "X";
            else if (k == j) std::cout << j;
            else             std::cout << "?";
            std::cout << (k < N ? " " : "");
        }
        std::cout << " ]\n";
        std::cout << "      (" << j << " is fixed, but " << i << " is NOT at position " << i << ")\n";
    }

    std::cout << "\n";
}

void task_2(
        const int N,
        const int trials,
        const int i,
        const int j,
        double& p_union,
        double& p_inter)
{
    if(trials <= 0)
    {
        throw std::invalid_argument("Amount of trials must be more than 0");
    }
    if (i < 1 || i > N || j < 1 || j > N)
        throw std::out_of_range("Indices i,j must be in [1.." + std::to_string(N) + "]");

    uint64_t count_union = 0;
    uint64_t count_inter = 0;

    bool same = (i == j);

    for(int k = 0; k < trials; ++k)
    {
        int value_at_i = std::rand()%N;
        bool Ai = (value_at_i == i - 1);

        bool Aj;

        if(same)
        {
            Aj = Ai;
        }
        else
        {
            int value_at_j = std::rand() % (N - 1);
            if (value_at_j >= value_at_i) value_at_j++;

            Aj = (value_at_j == j - 1);
        }

        if(Ai || Aj) ++ count_union;
        if(Ai && Aj) ++count_inter;
    }

    p_union = static_cast<double>(count_union)/trials;
    p_inter = static_cast<double>(count_inter)/trials;
}