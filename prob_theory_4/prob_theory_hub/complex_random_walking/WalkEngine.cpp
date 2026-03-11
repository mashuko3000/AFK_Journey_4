//
// Created by mvideo on 09/03/26.
//

#include "WalkEngine.hpp"
#include <cstdint>

WalkEngine::WalkEngine(const WalkConfig& config,
                    i_distribution_model* model)
                    : m_config(config),
                    model(model)
{
    validateConfig(m_config);
    if(!model)
    {
        throw std::invalid_argument("Distribution model pointer cannot be null");
    }
    m_results.reserve(m_config.M);
}

void WalkEngine::validateConfig(const WalkConfig& cfg) const
{
    if(cfg.M <= 0) throw std::invalid_argument("Number of trajectories (M) must be positive");
    if(cfg.K <= 0) throw std::invalid_argument("Maximum steps per trajectory (K) must be positive");
    if(std::abs(cfg.p) < 1e-10 || cfg.p > 1.0 + 1e-10) throw std::invalid_argument("Parameter p must be in [0, 1]");
    if(cfg.n <= 3) throw std::invalid_argument("Number of directions (n) must be at least 3");
    if (static_cast<uint64_t>(cfg.M) * static_cast<uint64_t>(cfg.K) > 1'000'000'000ULL)
    {
        throw std::invalid_argument("M × K is too large (> 1e9 steps total) — possible memory/time issue");
    }
}
WalkEngine::~WalkEngine()
{
    delete model;
}
bool WalkEngine::checkReturn(const std::complex<double>& pos) const
{
    const double eps = 1e-10;
    return std::abs(pos) < eps;
}
std::optional<std::complex<double>> WalkEngine::step()
{
    if(m_is_simulation_finished) return std::nullopt;

    if(m_current_step_idx == 0)
    {
        if(m_current_traj_idx >= m_config.M)
        {
            m_is_simulation_finished = true;
            return std::nullopt;
        }

        m_results.push_back(TrajectoryResult());
        m_results.back().points.push_back({0, 0});
        m_curr_pos = {0.0, 0.0};
    }

    if(!model) throw std::runtime_error("Distribution model is null during simulation");

    size_t xi = model->get_next_index();
    if (xi >= static_cast<size_t>(m_config.n))
    {
        throw std::runtime_error("Distribution model returned index >= n");
    }

    double angle = (2.0 * M_PI * static_cast<double>(xi)) / m_config.n;
    std::complex<double> step_val(m_config.p * std::cos(angle), m_config.p * std::sin(angle));

    m_curr_pos += step_val;
    m_current_step_idx++;

    auto& current_traj = m_results.back();
    current_traj.points.push_back(m_curr_pos);
    current_traj.steps_taken = m_current_step_idx;

    if(checkReturn(m_curr_pos))
    {
        current_traj.return_to_origin = true;
        m_current_step_idx = 0;
        m_current_traj_idx++;
    }
    else if (m_current_step_idx >= m_config.K)
    {
        current_traj.return_to_origin = false;
        m_current_step_idx = 0;
        m_current_traj_idx++;
    }

    return m_curr_pos;
}
std::vector<TrajectoryResult> WalkEngine::runAll()
{
    reset();
    while(!m_is_simulation_finished)
    {
        step();
    }

    return m_results;
}
void WalkEngine::reset()
{
    m_results.clear();
    m_results.reserve(m_config.M);
    m_curr_pos = {0, 0};
    m_current_step_idx = 0;
    m_current_traj_idx = 0;
    m_is_simulation_finished = false;
}
double WalkEngine::getEmpiricalProbability() const
{
    if(m_results.empty())
    {
        return 0.0;
    }
    return static_cast<double>(getSuccessfulReturnsCount()) / m_results.size();
}
int WalkEngine::getSuccessfulReturnsCount() const
{
    int count = 0;
    for(const auto& res : m_results)
    {
        if(res.return_to_origin) count++;
    }
    return count;
}
const std::vector<TrajectoryResult> WalkEngine::getAllResults()
{
    return m_results;
}
const WalkConfig WalkEngine::getConfig() const
{
    return m_config;
}
void WalkEngine::updateConfig(const WalkConfig& new_config)
{
    m_config = new_config;
    reset();
}
void WalkEngine::setDistribution(i_distribution_model* new_model)
{
    if (model) delete model;
    model = new_model;
    reset();
}