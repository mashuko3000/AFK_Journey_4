//
// Created by mvideo on 08/03/26.
//

#ifndef PROB_THEORY_HUB_RANDOMWALKING_HPP
#define PROB_THEORY_HUB_RANDOMWALKING_HPP

#include<QPointF>
#include <QVector>

#include"../math/distribution.hpp"

struct simulationConfig
{
    double initialY = 0.0;
    double stepH = 1.0;
    int stepsPerTrial = 1000;
    int trials = 100;
    int targetCrossingL = 1;
    int logLimitK = 5;
    QVector<double> s_values;
};

struct trialResult
{
    int id = 0;
    QVector<QPointF> path;
    int crossingCount = 0;
    bool isMatch = false;
};

struct simulationSummary
{
    double empiricalProbability = 0.0;
    int totalSimulation = 0;
    int totalSuccess = 0;
    QVector<trialResult> history;
};

class RandomWalking
{
private:
    simulationConfig m_config;
    simulationSummary m_summary;
    int m_iterationCounter = 0;
    i_distribution_model* model;

public:
    RandomWalking(const simulationConfig& conf, simulationSummary& res, i_distribution_model* mod);
    ~RandomWalking();

    void reset();

    simulationSummary get_summary() const {return m_summary;}

    simulationSummary runAll();
    trialResult runSingle();
};


#endif //PROB_THEORY_HUB_RANDOMWALKING_HPP
