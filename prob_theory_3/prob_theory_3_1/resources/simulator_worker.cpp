//
// Created by mvideo on 01/03/26.
//

#include "simulator_worker.hpp"

simulator_worker::simulator_worker(QObject *parent)
: QObject(parent), m_sim(nullptr), m_alpha(nullptr)
{
}
simulator_worker::~simulator_worker()
{
    cleanup_models();
}
void simulator_worker::reset_stats() {
    if(m_sim) {
        m_sim->reset();
        request_current_stats();
        emit progress_updated(0);
    }
}
void simulator_worker::create_models(const simulation_config& config)
{
    cleanup_models();
    try
    {
        i_distribution_model* model = nullptr;
        if(config.dist_type == distribution_type::discrete)
        {
            model = new discrete_distribution_model(config.weights);
        }
        else
        {
            throw std::invalid_argument("Not implemented yet");
        }
        m_alpha = new alphabet(config.alphabet.toStdString(), model);
        m_sim = new simulator(
                *m_alpha,
                config.M,
                config.n,
                config.pattern.toStdString(),
                config.d
        );
    }
    catch(const std::exception& e)
    {
        emit error_occurred(QString::fromStdString(e.what()));
        cleanup_models();
    }
}

void simulator_worker::cleanup_models()
{
    if(m_sim)
    {
        delete m_sim;
        m_sim = nullptr;
    }
    if(m_alpha)
    {
        delete m_alpha;
        m_alpha = nullptr;
    }
}

void simulator_worker::run_simulation(const simulation_config& config)
{
    if(m_isRunning) return;

    create_models(config);
    if (!m_sim) return;

    m_isRunning = true;
    m_stopRequested = false;

    size_t total = config.total_iterations;
    size_t chunk = std::max<size_t>(1, total / 100);

    for (size_t i = 0; i < total; ++i) {
        if (m_stopRequested) {
            break;
        }

        m_sim->run_step();

        if ((i + 1) % chunk == 0) {
            QCoreApplication::processEvents();

            emit progress_updated(static_cast<int>(((i + 1) * 100) / total));
            request_current_stats();
        }
    }

    m_isRunning = false;
    m_stopRequested = false;

    emit finished(m_sim->get_prob_a(), m_sim->get_prob_b(),
                  m_sim->get_prob_c(), m_sim->get_avg_connections());
    emit iteration_ready(m_sim->get_last_iteration());
}

void simulator_worker::run_full_simulation(const simulation_config& config)
{
    m_sim->reset();

    size_t total = config.total_iterations;
    size_t chunk = (total > 100) ? total / 100 : 1;

    for (size_t i = 0; i < total; ++i)
    {
        if(m_stopRequested)
        {
            m_stopRequested = false;
            break;
        }

        m_sim -> run_step();

        if ((i + 1) % chunk == 0 || i == total - 1)
        {
            emit progress_updated(static_cast<int>(((i + 1) * 100) / total));
            if ((i + 1) % (chunk * 5) == 0)
            {
                request_current_stats();
            }
        }
    }
    emit finished(m_sim->get_prob_a(),
                  m_sim->get_prob_b(),
                  m_sim->get_prob_c(),
                  m_sim->get_avg_connections());

    emit iteration_ready(m_sim->get_last_iteration());
}

void simulator_worker::run_single_step(const simulation_config& config) {
    if (!m_sim) create_models(config);
    if(!m_sim) return;

    if (m_sim->get_total_runs() >= config.total_iterations) return;

    m_sim->run_step();

    emit iteration_ready(m_sim->get_last_iteration());
    request_current_stats();

    double current = static_cast<double>(m_sim->get_total_runs());
    double total = static_cast<double>(config.total_iterations);

    if (total > 0) {
        int percent = static_cast<int>((current / total) * 100.0);
        emit progress_updated(std::min(percent, 100));
    }
}
void simulator_worker::stop()
{
    m_stopRequested = true;
}

void simulator_worker::run_batch_step(const simulation_config& config, int batchSize) {
    if (!m_sim) create_models(config);
    if (!m_sim) return;

    size_t current_runs = m_sim->get_total_runs();
    if (current_runs >= config.total_iterations) return;

    int effectiveBatch = std::min(static_cast<size_t>(batchSize), config.total_iterations - current_runs);

    for (int i = 0; i < effectiveBatch; ++i) {
        m_sim->run_step();
    }

    emit iteration_ready(m_sim->get_last_iteration());
    request_current_stats();

    double current = static_cast<double>(m_sim->get_total_runs());
    double total = static_cast<double>(config.total_iterations);
    int percent = static_cast<int>((current / total) * 100.0);

    emit progress_updated(std::min(percent, 100));
}
void simulator_worker::request_examples()
{
    if(m_sim)
    {
        emit best_success_ready(m_sim->get_example_success());
        emit best_fail_ready(m_sim->get_example_fail());
    }
}
void simulator_worker::request_current_stats()
{
    if(m_sim)
    {
        emit stats_updated(
                m_sim->get_prob_a(),
                m_sim->get_prob_b(),
                m_sim->get_prob_c(),
                m_sim->get_avg_connections(),
                m_sim->get_total_runs()
        );
    }
}