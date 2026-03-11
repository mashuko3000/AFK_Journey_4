//
// Created by mvideo on 08/03/26.
//

#include "RandomWalking.hpp"
#include <QVBoxLayout>
#include <QStyle>
#include <QPushButton>

RandomWalking::RandomWalking(const simulationConfig& conf, simulationSummary& res, i_distribution_model* mod)
: m_config(conf), m_summary(res), model(mod)
{
    m_iterationCounter = 0;
}
RandomWalking::~RandomWalking()
{
    delete model;
}

void RandomWalking::reset()
{
    m_summary.totalSimulation = 0;
    m_summary.totalSuccess = 0;
    m_summary.empiricalProbability = 0.0;
    m_summary.history.clear();
    m_iterationCounter = 0;
}

simulationSummary RandomWalking::runAll()
{
    reset();

    for (int i = 0; i < m_config.trials; ++i)
    {
        runSingle();
    }

    return m_summary;
}
trialResult RandomWalking::runSingle()
{
    trialResult result;
    result.id = ++m_iterationCounter;
    result.path.reserve(m_config.stepsPerTrial);

    double currentX = 0.0;
    double currentY = m_config.initialY;

    result.path.append(QPointF(currentX, currentY));

    for(int i = 0; i < m_config.stepsPerTrial; ++i)
    {
        size_t idx = model->get_next_index();
        if(idx >= static_cast<size_t>(m_config.s_values.size()))
        {
            idx = m_config.s_values.size() - 1;
        }

        double s = m_config.s_values[idx];

        currentX += m_config.stepH;
        currentY += s;

        QPointF nextPoint(currentX, currentY);

        double prevY = result.path.last().y();
        if ((prevY * currentY < 0) || (currentY == 0 && prevY != 0))
        {
            result.crossingCount++;
        }

        result.path.append(nextPoint);
    }

    result.isMatch = (result.crossingCount == m_config.targetCrossingL);

    m_summary.totalSimulation++;
    if (result.isMatch)
    {
        m_summary.totalSuccess++;
    }

    m_summary.empiricalProbability = static_cast<double>(m_summary.totalSuccess) / m_summary.totalSimulation;

    m_summary.history.append(result);
    if (m_summary.history.size() > m_config.logLimitK)
    {
        m_summary.history.removeFirst();
    }

    return result;
}