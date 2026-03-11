//
// Created by mvideo on 07/03/26.
//
#ifndef DRUNKARD_ENGINE_HPP
#define DRUNKARD_ENGINE_HPP

#include "../math/distribution.hpp"
#include <QVector>
#include <QRandomGenerator>
#include <QVector>

struct TrialResult {
    QVector<double> trajectory;
    bool fell = false;
    bool returned = false;
};

class DrunkardEngine {
private:
    double m_cafePos;
    double m_abyssPos;
    double m_pForward;
    double m_qBackward;

    int m_fallCount    = 0;
    int m_returnCount  = 0;
    int m_totalTrials  = 0;

public:
    DrunkardEngine(double cafePos, double abyssPos, double pForward);
    TrialResult runOneTrial(int maxSteps = 20000);
    void runSimulation(int trials, int maxStepsPerTrial = 20000);
    double getFallProbability() const
    {
        return m_totalTrials > 0 ? static_cast<double>(m_fallCount) / m_totalTrials : 0.0;
    }
    double getReturnProbability() const
    {
        return m_totalTrials > 0 ? static_cast<double>(m_returnCount) / m_totalTrials : 0.0;
    }
    void resetStatistics()
    {
        m_fallCount = m_returnCount = m_totalTrials = 0;
    }
    int getTotalTrials() const
    {
        return m_totalTrials;
    }

    void setParams(double cafePos, double abyssPos, double pForward) {
        m_cafePos = cafePos;
        m_abyssPos = abyssPos;
        m_pForward = pForward;
        m_qBackward = 1.0 - pForward;
    }
};

#endif //PROB_THEORY_HUB_DRUNKARDENGINE_HPP
