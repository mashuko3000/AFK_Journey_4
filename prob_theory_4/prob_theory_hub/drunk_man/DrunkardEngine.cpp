//
// Created by mvideo on 07/03/26.
//

#include "DrunkardEngine.hpp"

DrunkardEngine::DrunkardEngine(double cafePos, double abyssPos, double pForward)
        : m_cafePos(cafePos),
        m_abyssPos(abyssPos),
        m_returnCount(0),
        m_totalTrials(0),
        m_fallCount(0),
        m_pForward(pForward),
        m_qBackward(1 - pForward)
{
    if (cafePos <= m_abyssPos)
    {
        throw std::invalid_argument("Cafe position must be > abyss position");
    }
}
TrialResult DrunkardEngine::runOneTrial(int maxSteps)
{
    TrialResult result;
    double pos = m_cafePos;
    result.trajectory << pos;
    bool reachedCafeAgain = false;

    for (int step = 0; step < maxSteps; ++step)
    {
        double r = QRandomGenerator::global()->generateDouble();
        if (r < m_pForward) pos -= 1.0;
        else pos += 1.0;

        result.trajectory << pos;

        if (pos <= m_abyssPos + 0.1)
        {
            result.fell = true;
            m_fallCount++;
            m_totalTrials++;
            return result;
        }

        if (std::abs(pos - m_cafePos) < 0.5 && !reachedCafeAgain)
        {
            reachedCafeAgain = true;
            result.returned = true;
            m_returnCount++;
        }
    }

    m_totalTrials++;
    return result;
}
void DrunkardEngine::runSimulation(int trials, int maxStepsPerTrial)
{
    resetStatistics();
    for (int i = 0; i < trials; ++i)
    {
        runOneTrial(maxStepsPerTrial);
    }
}