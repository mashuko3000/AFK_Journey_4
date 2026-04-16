//
// Created by maria on 15.04.2026.
//

#ifndef INC_3_2_DISCRETERANDOMVARIABLE_HPP
#define INC_3_2_DISCRETERANDOMVARIABLE_HPP


#pragma once

#include <map>
#include <vector>
#include <string>
#include <random>

class DiscreteRandomVariable {
private:
    std::map<double, double> m_distribution;
    void validateDistribution() const;

public:
    DiscreteRandomVariable() = default;
    explicit DiscreteRandomVariable(const std::map<double, double>& dist);

    DiscreteRandomVariable operator+(const DiscreteRandomVariable& other) const;

    void loadFromFile(const std::string& filename);

    const std::map<double, double>& getDistribution() const { return m_distribution; }

    bool isEmpty() const { return m_distribution.empty(); }
    double generateRandomValue() const;

    double getMinValue() const;
    double getMaxValue() const;
};

#endif //INC_3_2_DISCRETERANDOMVARIABLE_HPP
