//
// Created by mvideo on 28/02/26.
//

#ifndef PROB_THEORY_3_1_SIMULATOR_HPP
#define PROB_THEORY_3_1_SIMULATOR_HPP

#include "alphabet.hpp"
#include "cluster.hpp"
#include "pattern_matcher.hpp"

struct iteration_data
{
    std::vector<cluster> clusters;
    std::vector<match_result> connections;
    bool all_connected = false;
    size_t connected_count = 0;
    bool none_connected = false;
};

class simulator {
private:
    alphabet& alpha;
    size_t count_claster_in_chapter;
    size_t len_cluster;
    size_t len_pattern;
    size_t d;
    string pattern;

    int total_connections_sum = 0;

    size_t total_runs = 0;
    size_t success_a = 0;
    size_t success_b = 0;
    size_t success_c = 0;

    iteration_data last_iteration;
    iteration_data best_success;
    iteration_data best_fail;

public:
    explicit simulator(alphabet& a,
              size_t count_clusters,
              size_t len_clus,
              string pat,
              size_t target_d);

    void run_step();
    void run_multiple(size_t count);
    void reset();

    double get_prob_a() const;
    double get_prob_b() const;
    double get_prob_c() const;
    double get_avg_connections() const;

    size_t get_total_runs() const {return total_runs;}
    const iteration_data& get_last_iteration() const {return last_iteration;}
    const iteration_data& get_example_success() const {return best_success;}
    const iteration_data& get_example_fail() const {return best_fail;}
};


#endif //PROB_THEORY_3_1_SIMULATOR_HPP
