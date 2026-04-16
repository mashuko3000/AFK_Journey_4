//
// Created by maria on 15.04.2026.
//

#include "DiscreteRandomVariable.hpp"

#include <fstream>
#include <cmath>
#include <stdexcept>

void DiscreteRandomVariable::validateDistribution() const {
    double sum = 0.0;
    for (const auto& [value, prob] : m_distribution) {
        if (prob < 0.0) throw std::invalid_argument("Probability cannot be negative.");
        sum += prob;
    }
    if (!m_distribution.empty() && std::abs(sum - 1.0) > 1e-6) {
        throw std::invalid_argument("Sum of probabilities must equal 1.");
    }
}

DiscreteRandomVariable::DiscreteRandomVariable(const std::map<double, double>& dist)
        : m_distribution(dist) {
    validateDistribution();
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

double DiscreteRandomVariable::generateRandomValue() const {
    if (m_distribution.empty()) return 0.0;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    double p = dis(gen);
    double cumulative = 0.0;

    for (const auto& [val, prob] : m_distribution) {
        cumulative += prob;
        if (p <= cumulative) return val;
    }
    return m_distribution.rbegin()->first;
}

double DiscreteRandomVariable::getMinValue() const {
    return m_distribution.empty() ? 0.0 : m_distribution.begin()->first;
}

double DiscreteRandomVariable::getMaxValue() const {
    return m_distribution.empty() ? 0.0 : m_distribution.rbegin()->first;
}