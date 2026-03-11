//
// Created by mvideo on 10/03/26.
//

#ifndef PROB_THEORY_HUB_STAIRCASESIMULATOR_HPP
#define PROB_THEORY_HUB_STAIRCASESIMULATOR_HPP

#include <vector>
#include <memory>
#include <QJsonObject>
#include <QJsonArray>

#include"../math/idistribution.hpp"

struct stairsConfig
{
    double M = 0.0;
    double h = 0.0;
    double tau = 0.0;
    int n = 0;
    int N = 0;
};

struct DistributionParams {
    QString type;
    QJsonObject props;
};

struct simulationResult
{
    double empiricalProbability = 0;
    size_t totalExperiments = 0;
    size_t successCount = 0;
    int stepsPerFigure = 0;
};

class StaircaseSimulator
{
public:
    explicit StaircaseSimulator(const stairsConfig& config, const DistributionParams& distParams);
    ~StaircaseSimulator();

public:
    StaircaseSimulator(const StaircaseSimulator&) = delete;
    StaircaseSimulator& operator=(const StaircaseSimulator&) = delete;
    StaircaseSimulator(StaircaseSimulator&&) = delete;
    StaircaseSimulator& operator=(StaircaseSimulator&&) = delete;

public:
    void updateConfig(const stairsConfig& config);
    void reset();
    void run();

    simulationResult getStats() const;
    bool isCalculated() const { return m_isCalculated; }

    std::vector<double> getTrial(size_t index) const;
    std::vector<size_t> getSuccessfulIndices() const;
    size_t getStoredFiguresCount() const { return m_allFigures.size(); }

private:
    void validateConfig(const stairsConfig& cfg) const;
    i_distribution_model* createModel(const DistributionParams& params, int n);
    void updateControlsState(bool calculated);
    void updatePlot(size_t trialIndex);

private:
    stairsConfig m_config;
    i_distribution_model* m_model;

    size_t m_successCount;
    bool m_isCalculated;

    std::vector<std::vector<double>> m_allFigures;
    std::vector<size_t> m_successIndices;

public:
    void clearModel();
    bool checkStrictlyIncreasing(const std::vector<double>& figure) const;
};


#endif //PROB_THEORY_HUB_STAIRCASESIMULATOR_HPP
