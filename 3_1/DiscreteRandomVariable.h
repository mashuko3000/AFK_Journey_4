//
// Created by maria on 15.04.2026.
//

#ifndef INC_3_1_DISCRETERANDOMVARIABLE_H
#define INC_3_1_DISCRETERANDOMVARIABLE_H

#pragma once

#include <map>
#include <vector>
#include <string>

class DiscreteRandomVariable {
private:
    std::map<double, double> m_distribution;

    void validateDistribution() const;

public:
    DiscreteRandomVariable() = default;

    explicit DiscreteRandomVariable(const std::map<double, double>& dist);
    DiscreteRandomVariable operator*(double scalar) const;
    DiscreteRandomVariable operator+(const DiscreteRandomVariable& other) const;
    DiscreteRandomVariable operator*(const DiscreteRandomVariable& other) const;

    double expectedValue() const;
    double variance() const;
    double skewness() const;
    double kurtosis() const;

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    const std::map<double, double>& getDistribution() const { return m_distribution; }
};

#endif //INC_3_1_DISCRETERANDOMVARIABLE_H
