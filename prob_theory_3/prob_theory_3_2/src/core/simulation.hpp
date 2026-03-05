//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_2_SIMULATION_HPP
#define PROB_THEORY_3_2_SIMULATION_HPP

#include<map>
#include"tree_model.hpp"

struct simulation_stats
{
    unsigned long long total_runs = 0;
    unsigned long long trapped_count = 0;
    std::map<node*, unsigned long long> leaf_hits;
    std::map<int, unsigned long long> length_dist;
};

struct iteration_data
{
    std::vector<node*> path;
    bool trapped = false;
    int final_length = 0;
    node* end_node = nullptr;
};

class simulation {
private:
    tree_model& m_tree;
    simulation_stats m_stats;

public:
    explicit simulation(tree_model& tree) : m_tree(tree) {};
    const simulation_stats& get_stats() const { return m_stats; }
    void reset() { m_stats = simulation_stats(); }
    iteration_data run_single_iteration();
};


#endif //PROB_THEORY_3_2_SIMULATION_HPP
