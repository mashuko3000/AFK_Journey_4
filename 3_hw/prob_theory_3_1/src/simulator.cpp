//
// Created by mvideo on 28/02/26.
//

#include "simulator.hpp"
#include <stdexcept>
#include<iostream>

simulator::simulator(alphabet& a,
                    size_t count_clusters,
                    size_t len_clus,
                    string pat,
                    size_t target_d)
                    :
        alpha(a),
        count_claster_in_chapter(count_clusters),
        len_cluster(len_clus),
        len_pattern(pat.length()),
        d(target_d),
        pattern(std::move(pat)),
        total_runs(0),
        success_a(0),
        success_b(0),
        success_c(0),
        last_iteration{},
        best_success{},
        best_fail{}
{
    if (count_claster_in_chapter < 2)
    {
        throw std::invalid_argument("Number of clusters in series must be at least 2");
    }

    if (len_cluster == 0)
    {
        throw std::invalid_argument("Cluster length must be greater than zero");
    }

    if (pattern.empty())
    {
        throw std::invalid_argument("Pattern cannot be empty");
    }
    if (len_pattern < 2)
    {
        throw std::invalid_argument("Pattern length must be at least 2 to allow non-trivial connections");
    }

    if (d > count_claster_in_chapter - 1)
    {
        throw std::invalid_argument("Target D cannot exceed the number of adjacent pairs (M-1)");
    }

    if (alpha.size() == 0)
    {
        throw std::invalid_argument("Alphabet is empty — cannot generate clusters");
    }

    reset();
}

void simulator::run_step()
{
    iteration_data current;
    current.clusters.reserve(count_claster_in_chapter);
    current.connections.reserve(count_claster_in_chapter - 1);

    for(size_t i = 0; i < count_claster_in_chapter; ++i)
    {
        cluster c(len_cluster);
        try
        {
            c.fill(alpha);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Failed to fill cluster: " << e.what() << std::endl;
            return;
        }
        current.clusters.push_back(std::move(c));
    }

    current.connected_count = 0;
    for (size_t i = 0; i + 1 < count_claster_in_chapter; ++i)
    {
        match_result res = pattern_matcher::are_connectable(
                current.clusters[i],
                current.clusters[i + 1],
                pattern
        );

        if (res.connected)
        {
            ++current.connected_count;
        }

        current.connections.push_back(std::move(res));
    }

    current.all_connected  = (current.connected_count == count_claster_in_chapter - 1);
    current.none_connected = (current.connected_count == 0);

    ++total_runs;
    total_connections_sum += current.connected_count;

    if (current.all_connected)  ++success_a;
    if (current.connected_count == d) ++success_b;
    if (current.none_connected) ++success_c;

    last_iteration = current;

    if (current.all_connected && current.connected_count > best_success.connected_count)
    {
        best_success = current;
    }

    if (current.none_connected && current.connected_count < best_fail.connected_count)
    {
        best_fail = current;
    }
}
void simulator::run_multiple(size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        run_step();
        if ((i + 1) % 10000 == 0)
        {
            std::cout << "Progress: " << (i + 1) << " / " << count << "\r" << std::flush;
        }
    }
    std::cout << std::endl;
}
void simulator::reset()
{
    total_connections_sum = 0;

    total_runs = 0;
    success_a = 0;
    success_b = 0;
    success_c = 0;

    last_iteration = iteration_data();
    best_success = iteration_data();
    best_fail = iteration_data();
}

double simulator::get_prob_a() const
{
    return total_runs > 0 ? static_cast<double>(success_a) / total_runs : 0.0;
}
double simulator::get_prob_b() const
{
    return total_runs > 0 ? static_cast<double>(success_b) / total_runs : 0.0;
}
double simulator::get_prob_c() const
{
    return total_runs > 0 ? static_cast<double>(success_c) / total_runs : 0.0;
}
double simulator::get_avg_connections() const
{
    return total_runs > 0 ? static_cast<double>(total_connections_sum) / total_runs : 0.0;
}