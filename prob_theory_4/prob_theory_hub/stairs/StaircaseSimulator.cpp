//
// Created by mvideo on 10/03/26.
//

#include "StaircaseSimulator.hpp"
#include "../math/distribution.hpp"

void StaircaseSimulator::validateConfig(const stairsConfig& cfg) const
{
    if(cfg.M <= 0) throw std::invalid_argument("Length of line segment (M) must be positive");
    if(cfg.h <= 0) throw std::invalid_argument("The number representing the number of segments into which the original one should be divided (K) must be positive");
    if(cfg.tau <= 0) throw std::invalid_argument("Number of tao (tau) must be at least 3");
    if(cfg.n <= 1) throw std::invalid_argument("Number of range (n) must be at least 3");
    if(cfg.N <= 10) throw std::invalid_argument("Number of trials (N) is too small");
}

StaircaseSimulator::StaircaseSimulator(const stairsConfig &config, const DistributionParams& distParams)
: m_model(nullptr), m_successCount(0), m_isCalculated(false)
{
    validateConfig(config);
    m_config = config;

    m_model = createModel(distParams, m_config.n);
}

StaircaseSimulator::~StaircaseSimulator()
{
    if(m_model)
    {
        delete m_model;
    }
}

i_distribution_model* StaircaseSimulator::createModel(const DistributionParams& params, int n)
{
    size_t statesCount = static_cast<size_t>(n + 1);
    QString type = params.type.toLower();

    if (type == "uniform")
    {
        return new uniform_distribution_model(statesCount);
    }
    else if (type == "binomial")
    {
        double p = params.props.value("p").toDouble(0.5);
        return new binomial_distribution(p, statesCount);
    }
    else if (type == "geometric")
    {
        double p = params.props.value("p").toDouble(0.5);
        return new geometric_distribution(p, statesCount);
    }
    else if (type == "triangular")
    {
        return new triangular(statesCount);
    }

    return new uniform_distribution_model(statesCount);
}

void StaircaseSimulator::run()
{
    int K = static_cast<int>(std::floor(m_config.M / m_config.h));
    reset();

    if(K > (m_config.n + 1))
    {
        m_isCalculated = true;
        return;
    }

    m_allFigures.reserve(static_cast<size_t>(m_config.N));

    for(int i = 0; i < m_config.N; ++i)
    {
        std::vector<double> currentFigure;
        currentFigure.reserve(static_cast<size_t>(K));
        for(int j = 0; j < K; ++j)
        {
            double value = static_cast<double>(m_model->get_next_index()) * m_config.tau;
            currentFigure.push_back(value);
        }
        if(checkStrictlyIncreasing(currentFigure))
        {
            m_successCount++;
            m_successIndices.push_back(static_cast<size_t>(i));
        }
        m_allFigures.push_back(std::move(currentFigure));
    }
    m_isCalculated = true;
}

bool StaircaseSimulator::checkStrictlyIncreasing(const std::vector<double>& figure) const
{
    if(figure.empty()) return false;
    if(figure.size() < 2) return true;

    for(size_t i = 1; i < figure.size(); ++i)
    {
        if(figure[i] <= figure[i - 1])
        {
            return false;
        }
    }
    return true;
}

void StaircaseSimulator::reset()
{
    m_allFigures.clear();
    m_successIndices.clear();
    m_successCount = 0;
    m_isCalculated = false;
}

void StaircaseSimulator::updateConfig(const stairsConfig& config)
{
    validateConfig(config);
    m_config = config;
    reset();
}

simulationResult StaircaseSimulator::getStats() const
{
    simulationResult res;
    res.totalExperiments = m_config.N;
    res.successCount = m_successCount;
    res.stepsPerFigure = static_cast<int>(std::floor(m_config.M / m_config.h));

    if (res.totalExperiments > 0)
    {
        res.empiricalProbability = static_cast<double>(m_successCount) / res.totalExperiments;
    }

    return res;
}

std::vector<double> StaircaseSimulator::getTrial(size_t index) const
{
    if(index >= m_allFigures.size())
    {
        return{};
    }
    return m_allFigures[index];
}

std::vector<size_t> StaircaseSimulator::getSuccessfulIndices() const
{
    return m_successIndices;
}

void StaircaseSimulator::clearModel()
{
    if(m_model)
    {
        delete m_model;
        m_model = nullptr;
    }
}