//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_1_SIMULATOR_WORKER_HPP
#define PROB_THEORY_3_1_SIMULATOR_WORKER_HPP

#include <QObject>
#include <QString>
#include<QCoreApplication>
#include <vector>
#include "../src/simulator.hpp"

enum class distribution_type {
    uniform,
    discrete
};

struct simulation_config
{
    QString alphabet;
    std::vector<double>weights;
    distribution_type dist_type;

    QString pattern;
    size_t M;
    size_t n;
    size_t d;
    size_t total_iterations;
};

class simulator_worker : public QObject
{
    Q_OBJECT;

public:
    explicit simulator_worker(QObject *parent = nullptr);
    ~simulator_worker();

public slots:
    void run_simulation(const simulation_config& config);
    void run_single_step(const simulation_config& config);
    void stop();
    void reset_stats();
    void request_examples();
    void request_current_stats();
    size_t get_total_runs() const {
        return m_sim ? m_sim->get_total_runs() : 0;
    }
    void run_batch_step(const simulation_config& config, int batchSize);

signals:
    void progress_updated(int percent);
    void finished(double probA, double probB, double probC, double avgConn);
    void stats_updated(double probA, double probB, double probC, double avgConn, size_t currentIter);
    void iteration_ready(const iteration_data& data);
    void best_success_ready(const iteration_data& data);
    void best_fail_ready(const iteration_data& data);
    void error_occurred(const QString& message);


private:
    simulator* m_sim = nullptr;
    alphabet* m_alpha = nullptr;

    bool m_isRunning = false;
    bool m_stopRequested = false;
    size_t m_currentIteration = 0;

    void create_models(const simulation_config& config);
    void cleanup_models();

    void run_full_simulation(const simulation_config& config);
};


#endif //PROB_THEORY_3_1_SIMULATOR_WORKER_HPP
