#include<iostream>
#include<stdexcept>
#include<cstdint>
#include<cstdlib>
#include<ctime>
#include<iomanip>

void task_3(const int k,
            const int trials,
            double& p_before_k,
            double& p_even);
double theo_p_before_k(int k);
double theo_p_even(int k);

void print_sample_space_examples(int max_n = 10) {
    std::cout << "\nSample space examples (sequences up to length " << max_n << "):\n";
    for (int n = 2; n <= max_n; ++n)
    {
        std::cout << "Length " << n << ": ";
        std::cout << "H";
        char prev = 'H';
        for (int i = 2; i < n; ++i)
        {
            char next = (prev == 'H') ? 'T' : 'H';
            std::cout << next;
            prev = next;
        }
        std::cout << prev << ", ";
        std::cout << "T";
        prev = 'T';
        for (int i = 2; i < n; ++i)
        {
            char next = (prev == 'T') ? 'H' : 'T';
            std::cout << next;
            prev = next;
        }
        std::cout << prev << "\n";
    }
    std::cout << "( ... )\n\n";
}

int main(int argc, char* argv[])
{
    try
    {
        if(argc < 2 || argc > 3)
        {
            throw std::invalid_argument("Usage: " + std::string(argv[0]) + " [k] [trials]");
        }

        int k = std::stoi(argv[1]);
        if(k < 2) throw std::invalid_argument("k must be >= 2");

        int trials = 1000000;
        if(argc == 3)
        {
            trials = std::max(1000000, std::atoi(argv[2]));
        }

        std::srand((std::time(0)));

        double theo_b = theo_p_before_k(k);
        double theo_e = theo_p_even(k);

        double emp_before = 0.0;
        double emp_even = 0.0;

        task_3(k, trials, emp_before, emp_even);

        print_sample_space_examples(10);

        std::cout << "THEO:\n";
        std::cout << "  P(stop before k-th throw) = " << std::fixed << std::setprecision(8) << theo_b << "\n";
        std::cout << "  P(even number of throws)  = " << std::fixed << std::setprecision(8) << theo_e<< "\n\n";

        std::cout << "EMP:\n";
        std::cout << "  P(before k) ≈ " << std::fixed << std::setprecision(8) << emp_before << "\n";
        std::cout << "  P(even)     ≈ " << std::fixed << std::setprecision(8) << emp_even << "\n\n";
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << "\nError: " << e.what() << "\n\n";
        return 1;
    }
}

double theo_p_before_k(int k)
{
    if(k < 3) return 0.0;
    if (k - 2 >= 63) return 1.0;
    return 1.0 - 1.0 / (1LL<<(k-2));
}

double theo_p_even(int k)
{
    return 2.0 / 3.0;
}

void task_3(const int k,
            const int trials,
            double& p_before_k,
            double& p_even)
{
    if (trials <= 0) throw std::invalid_argument("Trials must be > 0");
    if (k < 2) throw std::invalid_argument("k must be >= 2");

    uint64_t before = 0;
    uint64_t even = 0;

    for(int i = 0; i < trials; ++i)
    {
        int length = 0;
        int prev = -1;

        while(true)
        {
            int curr = std::rand() % 2;
            ++length;

            if(length >= 2 && curr == prev)
            {
                if(length < k) ++before;
                if(!(length & 1)) ++even;
                break;
            }
            prev = curr;
        }
    }

    p_before_k = static_cast<double>(before) / trials;
    p_even = static_cast<double>(even) / trials;
}