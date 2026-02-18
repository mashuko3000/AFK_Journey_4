#include "part_2/1_task/1_task.hpp"
#include <iostream>
#include <string>


void print_stats(const char* label, const stat& emp, const stat& theo) {
    std::cout << label << "\n"
              << "  1st:  " << emp.first << " VS " << theo.first << "\n"
              << "  2nd:  " << emp.second << " VS " << theo.second << "\n"
              << "  both:  " << emp.both << " VS " << theo.both << "\n\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <N> <simulations>\n";
        return 1;
    }
    std::srand(std::time(0));
    try
    {
        int N = std::stoi(argv[1]);
        int64_t trials = std::stoll(argv[2]);

        if (N <= 2)
        {
            throw std::invalid_argument("N must be at least 2");
        }
        if(trials < 1 || trials > 1000000000)
        {
            throw std::out_of_range("Trials must be between 1 and 1,000,000,000");
        }

        std::cout << "\nStarting simulation:\n";
        std::cout << "  Alphabet size N  = " << N << "\n";
        std::cout << "  Simulations      = " << trials << "\n\n";

        stat empA, theoA, empB, theoB;
        task_1(N, trials, false, empA, theoA);
        task_1(N, trials, true, empB, theoB);

        std::cout << "=== RESULTS (EMP VS THEO) ===\n";
        print_stats("a) without shifting", empA, theoA);
        print_stats("b_ with shifting", empB, theoB);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error : " << e.what() << "\n";
    }
    catch (...)
    {
        std::cerr << "Unknown error" << "\n";
    }

    return 0;
}