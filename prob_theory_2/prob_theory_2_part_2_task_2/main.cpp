#include <iostream>
#include <stdexcept>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>

int main(int argc, char* argv[]) {
    try{
        if (argc < 2)
        {
            throw std::invalid_argument("");
        }

        int trials;

        trials = std::stoi(argv[1]);
        if (trials <= 0) throw std::runtime_error("count of trials must be more than 0");

        const int keys = 10;
        std::vector<int>stat(keys + 1, 0);

        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        for (int i = 0; i < trials; ++i)
        {
            int winning_position = (std::rand() % keys) + 1;
            stat[winning_position]++;
        }

        std::cout << "Simulation Results (n = " << keys << ", trials = " << trials << ")\n";
        std::cout << std::string(50, '-') << "\n";
        std::cout << std::left << std::setw(12) << "Attempt #"
                  << std::setw(15) << "Successes"
                  << "Empirical Prob." << std::endl;
        std::cout << std::string(50, '-') << "\n";

        for (int i = 1; i <= keys; ++i)
        {
            double empirical_prob = static_cast<double>(stat[i]) / trials;
            std::cout << std::left << std::setw(12) << i
                      << std::setw(15) << stat[i]
                      << std::fixed << std::setprecision(5) << empirical_prob << std::endl;
        }

        std::cout << std::string(50, '-') << "\n";
        std::cout << "Theoretical Probability (1/n): " << (1.0 / keys) << std::endl;

    } catch (const std::invalid_argument& e) {
        std::cerr << "Input Error: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Input Error: trials value is out of range for an integer." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
    }
    return 0;
}

