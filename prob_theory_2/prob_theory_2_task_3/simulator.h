#ifndef SIMULATOR_H
#define SIMULATOR_H

#include<string>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<cstdint>
#include<chrono>


enum class spread_mode_t
{
    single_person = 0,
    group = 1
};

struct simulation_parameters_t
{
    int population = 0;
    int max_steps = 0;
    int group_size = 0;
    spread_mode_t mode = spread_mode_t::single_person;
    unsigned seed = 0;
    bool stop_on_cycle = true;
    bool stop_on_return_to_source = true;
};

struct simulator_config_t
{
    simulation_parameters_t params;
    int num_trials = 100000;
    bool keep_details = false;
};

struct one_result_t {
    int run_number = 0;
    bool returned_to_source = false;
    bool had_any_repeat = false;
    bool reached_max_steps = false;
    int steps_done = 0;
    int unique_heard = 0;
    std::vector<int> path;
    std::chrono::milliseconds duration{0};
    std::string stop_reason;
};

struct summary_t {
    uint64_t total_runs = 0;
    uint64_t good_for_a = 0;
    uint64_t good_for_b = 0;
    uint64_t reached_everyone = 0;

    double p_A = 0.0;
    double p_B = 0.0;
    double p_full = 0.0;

    double avg_unique = 0.0;
    double avg_steps = 0.0;

    std::chrono::milliseconds totalTime{0};
};

class rumor_simulator final {
public:
    //Rule of 5
    explicit rumor_simulator(simulator_config_t config = {});
    ~rumor_simulator();

    rumor_simulator(const rumor_simulator&) = delete;
    rumor_simulator& operator=(const rumor_simulator&) = delete;

    rumor_simulator(rumor_simulator&&) = delete;
    rumor_simulator& operator=(rumor_simulator&&) = delete;

public:
    void run();
    void reset();

    const summary_t& get_summary() const;
    double get_prob_a() const;
    double get_prob_b() const;

    const std::vector<one_result_t>& get_all_runs() const;
    void set_config(simulator_config_t& new_config);

private:
    simulator_config_t config;
    summary_t summary;
    std::vector<one_result_t> detailed_runs;

    one_result_t simulate_one_run(int run_index);
    void accumulate_result(const one_result_t& res);
};


#endif // SIMULATOR_H
