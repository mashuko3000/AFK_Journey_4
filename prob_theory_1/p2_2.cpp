#include "part_2/2_task/2_task.hpp"

int main(int argc, char* argv[])
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    try
    {
        if (argc < 2)
        {
            throw std::runtime_error("Not enough arguments. Usage: " + std::string(argv[0]) + " <N> [trials]");
        }

        int N;
        try
        {
            N = std::stoi(argv[1]);
        }
        catch (...)
        {
            throw std::invalid_argument("N must be an integer.");
        }

        if (N < 2)
        {
            throw std::out_of_range("N must be >= 2.");
        }

        long long trials = 3000000LL;
        if (argc >= 3)
        {
            try
            {
                trials = std::stoll(argv[2]);
            }
            catch (...)
            {
                throw std::invalid_argument("Number of trials must be an integer.");
            }
        }

        if (trials <= 0)
        {
            throw std::out_of_range("Number of trials must be a positive number.");
        }

        std::cout << "Parameters: N = " << N << ", Trials = " << trials << "\n\n";

        int pairs[][2] = {
                {1, 1},
                {1, 2},
                {N/2 + 1, N}
        };

        for (auto& p : pairs)
        {
            int i = p[0];
            int j = p[1];
            if (i > N || j > N || i < 1 || j < 1) continue;

            double pA = theo_p_fixed(i, N);
            double pB = theo_p_fixed(j, N);
            double p_inter_anal = theo_p_intersection(i, j, N);
            double p_union_anal = theo_p_union(i, j, N);

            double p_union_mc   = 0.0;
            double p_inter_mc   = 0.0;

            task_2(N, static_cast<int>(trials), i, j, p_union_mc, p_inter_mc);

            std::cout << "\nEvents A_" << i << " (at position " << i << ") and A_" << j << " (at position " << j << ")\n\n";

            std::cout << "THEO:\n";
            std::cout << "  P(Ai) = " << std::fixed << std::setprecision(6) << pA << "\n";
            std::cout << "  P(Ai ∩ Aj) = " << std::fixed << std::setprecision(8) << p_inter_anal << "\n";
            std::cout << "  P(Ai ∪ Aj) = " << std::fixed << std::setprecision(8) << p_union_anal
                      << " (Theorem: P(A) + P(B) - P(AB))\n\n";

            std::cout << "EMP:\n";
            std::cout << "  P(Ai ∩ Aj) ≈ " << std::fixed << std::setprecision(8) << p_inter_mc << "\n";
            std::cout << "  P(Ai ∪ Aj) ≈ " << std::fixed << std::setprecision(8) << p_union_mc << "\n\n";

            std::cout << "Favorable outcomes logic:\n";
            print_favorable_logic(N, i, j);
            std::cout << "\n";
        }

    } catch (const std::invalid_argument& e) {
        std::cerr << "Argument error: " << e.what() << "\n";
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Range error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unexpected system error.\n";
        return 1;
    }

    std::cout << "Computations completed successfully.\n";
    return 0;
}