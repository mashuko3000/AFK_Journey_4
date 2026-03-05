//
// Created by mvideo on 01/03/26.
//

#include "simulation.hpp"

iteration_data simulation::run_single_iteration()
{
    iteration_data result;
    node* current = m_tree.get_root();

    while (current)
    {
        result.path.push_back(current);
        current->visit_count++;

        if (current->is_leaf)
        {
            result.end_node = current;
            m_stats.leaf_hits[current]++;
            break;
        }

        double r = static_cast<double>(std::rand()) / RAND_MAX;
        if (r < current->p_stay)
        {
            current->stay_count++;
            m_stats.trapped_count++;
            result.trapped = true;
            result.end_node = current;
            break;
        }

        size_t next_idx = current->dist_model->get_next_index();
        if (next_idx >= current->children.size()) next_idx = current->children.size() - 1;

        current = current->children[next_idx];
    }

    result.final_length = static_cast<int>(result.path.size());
    m_stats.length_dist[result.final_length]++;
    m_stats.total_runs++;

    return result;
}
