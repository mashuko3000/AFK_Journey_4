#include "simulator.h"

rumor_simulator::rumor_simulator(simulator_config_t config)
    : config(config), summary{}, detailed_runs{}
{
    if(config.params.seed == 0) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    } else {
        std::srand(config.params.seed);
    }
}

rumor_simulator::~rumor_simulator() {}

void rumor_simulator::run() {
    reset();
    auto start_time = std::chrono::steady_clock::now();

    for (int i = 0; i < config.num_trials; ++i) {
        one_result_t result = simulate_one_run(i);
        accumulate_result(result);
    }

    auto end_time = std::chrono::steady_clock::now();
    summary.totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    if (summary.total_runs > 0) {
        summary.p_A = static_cast<double>(summary.good_for_a) / summary.total_runs;
        summary.p_B = static_cast<double>(summary.good_for_b) / summary.total_runs;
        summary.avg_unique /= summary.total_runs;
        summary.avg_steps /= summary.total_runs;
    }
}

one_result_t rumor_simulator::simulate_one_run(int run_index) {
    one_result_t res;
    res.run_number = run_index;
    auto t0 = std::chrono::steady_clock::now();

    const int n_total = config.params.population + 1;
    const int r_steps = config.params.max_steps;
    const int N_group = config.params.group_size;

    std::vector<bool> known(n_total, false);
    known[0] = true;

    int current_source = 0;
    bool hit_original_source = false;
    bool repeat_occurred = false;
    int unique_informed = 1;

    for (int step = 0; step < r_steps; ++step) {
        std::vector<int> receivers;

        if (config.params.mode == spread_mode_t::single_person) {
            int target;
            do {
                target = std::rand() % n_total;
            } while (target == current_source);
            receivers.push_back(target);
        }
        else {
            std::vector<int> pool;
            for(int i = 0; i < n_total; ++i) {
                if(i != current_source) pool.push_back(i);
            }

            for (int i = 0; i < N_group && !pool.empty(); ++i) {
                int idx = std::rand() % pool.size();
                receivers.push_back(pool[idx]);
                pool.erase(pool.begin() + idx);
            }
        }

        for (int person : receivers) {
            if (person == 0) hit_original_source = true;
            if (known[person]) repeat_occurred = true;

            if (!known[person]) {
                known[person] = true;
                unique_informed++;
            }
        }

        if (!receivers.empty()) {
            current_source = receivers[std::rand() % receivers.size()];
        }
    }

    res.returned_to_source = hit_original_source;
    res.had_any_repeat = repeat_occurred;
    res.steps_done = r_steps;
    res.unique_heard = unique_informed;
    res.stop_reason = (hit_original_source || repeat_occurred) ? "Condition violated" : "Completed r steps";

    auto t1 = std::chrono::steady_clock::now();
    res.duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    return res;
}

void rumor_simulator::accumulate_result(const one_result_t& res) {
    summary.total_runs++;
    if (!res.returned_to_source) summary.good_for_a++;
    if (!res.had_any_repeat) summary.good_for_b++;
    summary.avg_unique += res.unique_heard;
    summary.avg_steps += res.steps_done;
    if (config.keep_details) detailed_runs.push_back(res);
}

void rumor_simulator::reset() {
    summary = summary_t{};
    detailed_runs.clear();
}

const summary_t& rumor_simulator::get_summary() const { return summary; }
const std::vector<one_result_t>& rumor_simulator::get_all_runs() const { return detailed_runs; }

