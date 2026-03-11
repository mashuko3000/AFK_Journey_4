//
// Created by mvideo on 09/03/26.
//

#ifndef PROB_THEORY_HUB_WALKENGINE_HPP
#define PROB_THEORY_HUB_WALKENGINE_HPP

#include<vector>
#include<complex>
#include<optional>
#include "../math/distribution.hpp"

struct WalkConfig
{
    int M;
    int K;
    double p;
    int n;

};

struct TrajectoryResult
{
    std::vector<std::complex<double>> points;
    bool return_to_origin = false;
    int steps_taken = 0;
};

class WalkEngine {
private:
    i_distribution_model* model;
    WalkConfig m_config;
    std::vector<TrajectoryResult> m_results;

private:
    std::complex<double> m_curr_pos{0, 0};
    int m_current_step_idx = 0;
    int m_current_traj_idx = 0;
    bool m_is_simulation_finished = false;

private:
    bool checkReturn(const std::complex<double>& pos) const;

public:
    explicit WalkEngine(const WalkConfig& config,
                        i_distribution_model* model);
    ~WalkEngine();

    WalkEngine(const WalkConfig& other) = delete;
    WalkConfig& operator=(const WalkConfig& other) = delete;

    WalkEngine(WalkConfig&& other) = delete;
    WalkConfig& operator=(WalkConfig&& other) = delete;
public:
    std::optional<std::complex<double>>step();
    std::vector<TrajectoryResult>runAll();
    void reset();

public:
    double getEmpiricalProbability() const;

    int getSuccessfulReturnsCount() const;
    const std::vector<TrajectoryResult> getAllResults();
    const WalkConfig getConfig() const;

    void validateConfig(const WalkConfig& cfg) const;
    int getCurrentTrajIndex() const { return m_current_traj_idx; }

public:
    void updateConfig(const WalkConfig& new_config);
    void setDistribution(i_distribution_model* model);
};



#endif //PROB_THEORY_HUB_WALKENGINE_HPP
