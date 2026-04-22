//
// Created by maria on 22.04.2026.
//

#ifndef HW_10_LONGESTCYCLE_HPP
#define HW_10_LONGESTCYCLE_HPP

#include"Graph.hpp"

void findLongestCycle(int start, int curr, int mask, int edges, int weight, int& max_e, int& max_w, const Graph& g) {
    for (int v = 0; v < g.n; ++v)
    {
        int w = g.getEdge(curr, v);
        if (w <= 0) continue;

        if (v == start && edges >= 2)
        {
            max_e = std::max(max_e, edges + 1);
            max_w = std::max(max_w, weight + w);
        }
        else if (!(mask & (1 << v)))
        {
            findLongestCycle(start, v, mask | (1 << v), edges + 1, weight + w, max_e, max_w, g);
        }
    }
}

#endif //HW_10_LONGESTCYCLE_HPP
