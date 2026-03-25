//
// Created by mvideo on 24/03/26.
//

#ifndef PROB_THEORY_6_IEXPERIMENT_HPP
#define PROB_THEORY_6_IEXPERIMENT_HPP

#include<string>
#include<vector>

template<typename T>
class IExperiment
{
public:
    IExperiment() = default;
    virtual ~IExperiment() = default;

    IExperiment(const IExperiment& other) = default;
    IExperiment& operator=(const IExperiment& other) = default;
    IExperiment(IExperiment&&) noexcept = default;
    IExperiment& operator=(IExperiment&&) noexcept = default;

public:
    virtual T runEmpirical(long long trials) = 0;
    virtual T calculateTheoretical() const = 0;
    virtual std::string getName() const = 0;
};

struct NeedleResult
{
    double x;
    double phi;
    bool crosses;
};

class ISimulation
{
public:
    virtual ~ISimulation() = default;
    virtual void run(int count, double d, double L) = 0;
    virtual double getEmpiricalProb() const = 0;
    virtual double getTheoryProb(double d, double L) const = 0;
    virtual const std::vector<NeedleResult>& getResult() const = 0;
};

#endif //PROB_THEORY_6_IEXPERIMENT_HPP
