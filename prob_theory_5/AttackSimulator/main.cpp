#include <iostream>
#include <iomanip>
#include <cmath>
#include "VernamAttack.hpp"

int main() {
    const double N = 65536.0;
    const double target_prob = 0.5;

    int theoretical_k = std::ceil(std::sqrt(N * std::log(2)));

    std::cout << "=== Math Theory ===\n";
    std::cout << "Space size N = " << N << "\n";
    std::cout << "Required k for >50% collision prob: " << theoretical_k << "\n\n";

    uint16_t secret_key = 4242;
    size_t trials = 100000;

    auto cipher = std::make_shared<VernamCipher>(secret_key);
    auto generator = std::make_shared<Random16BitSetGenerator>();
    BirthdayAttackSimulator simulator(generator, cipher);

    std::cout << "--- Empirical Simulation ---\n";
    std::cout << "Running " << trials << " trials for k = " << theoretical_k << "...\n";

    double emp_prob = simulator.runSimulator(theoretical_k, trials);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Empirical Probability R(k, N): " << emp_prob << "\n";

    if (emp_prob > target_prob) {
        std::cout << "Success! Probability is > 0.5, confirming the theory.\n";
    }

    return 0;
}