#include "DiscreteRandomVariable.h"
#include <fstream>
#include <cmath>
#include <stdexcept>
#include <iostream>

void DiscreteRandomVariable::validateDistribution() const {
    double sum = 0.0;
    for (const auto& [value, prob] : m_distribution) {
        if (prob < 0.0) throw std::invalid_argument("Probability cannot be negative.");
        sum += prob;
    }
    if (std::abs(sum - 1.0) > 1e-6) {
        throw std::invalid_argument("Sum of probabilities must equal 1.");
    }
}

DiscreteRandomVariable::DiscreteRandomVariable(const std::map<double, double>& dist)
        : m_distribution(dist) {
    validateDistribution();
}

DiscreteRandomVariable DiscreteRandomVariable::operator*(double scalar) const {
    std::map<double, double> newDist;
    for (const auto& [val, prob] : m_distribution) {
        newDist[val * scalar] += prob; // += на случай, если scalar == 0 и ключи схлопнутся
    }
    return DiscreteRandomVariable(newDist);
}

DiscreteRandomVariable DiscreteRandomVariable::operator+(const DiscreteRandomVariable& other) const {
    std::map<double, double> newDist;
    for (const auto& [val1, prob1] : m_distribution) {
        for (const auto& [val2, prob2] : other.m_distribution) {
            newDist[val1 + val2] += prob1 * prob2;
        }
    }
    return DiscreteRandomVariable(newDist);
}

DiscreteRandomVariable DiscreteRandomVariable::operator*(const DiscreteRandomVariable& other) const {
    std::map<double, double> newDist;
    for (const auto& [val1, prob1] : m_distribution) {
        for (const auto& [val2, prob2] : other.m_distribution) {
            newDist[val1 * val2] += prob1 * prob2;
        }
    }
    return DiscreteRandomVariable(newDist);
}

double DiscreteRandomVariable::expectedValue() const {
    double ev = 0.0;
    for (const auto& [val, prob] : m_distribution) ev += val * prob;
    return ev;
}

double DiscreteRandomVariable::variance() const {
    double ev = expectedValue();
    double var = 0.0;
    for (const auto& [val, prob] : m_distribution) {
        var += std::pow(val - ev, 2) * prob;
    }
    return var;
}

double DiscreteRandomVariable::skewness() const {
    double var = variance();
    if (var == 0.0) return 0.0;
    double stddev = std::sqrt(var);
    double ev = expectedValue();
    double skew = 0.0;
    for (const auto& [val, prob] : m_distribution) {
        skew += std::pow(val - ev, 3) * prob;
    }
    return skew / std::pow(stddev, 3);
}

double DiscreteRandomVariable::kurtosis() const {
    double var = variance();
    if (var == 0.0) return 0.0;
    double stddev = std::sqrt(var);
    double ev = expectedValue();
    double kurt = 0.0;
    for (const auto& [val, prob] : m_distribution) {
        kurt += std::pow(val - ev, 4) * prob;
    }
    // Коэффициент эксцесса (избыток)
    return (kurt / std::pow(stddev, 4)) - 3.0;
}

void DiscreteRandomVariable::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Could not open file for writing.");
    for (const auto& [val, prob] : m_distribution) {
        out << val << " " << prob << "\n";
    }
}

void DiscreteRandomVariable::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Could not open file for reading.");

    std::map<double, double> newDist;
    double val, prob;
    while (in >> val >> prob) {
        newDist[val] += prob;
    }

    m_distribution = newDist;
    validateDistribution();
}