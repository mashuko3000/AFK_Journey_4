//
// Created by maria on 22.04.2026.
//

#ifndef HW_10_MSTWEIGHT_HPP
#define HW_10_MSTWEIGHT_HPP

#include "Graph.hpp"

double getMSTWeight(const Graph& g)
{
    int n = g.n;
    if(n == 0) return 0;

    std::vector<int> minE(n, 1e9);
    std::vector<bool> used(n, false);

    minE[0] = 0;
    double total = 0;

    for(int i = 0; i < n; ++i)
    {
        int v = -1;
        for(int j = 0; j < n; ++j)
        {
            if(!used[j] && (v == -1 || minE[j] < minE[v]))
            {
                v = j;
            }
        }
        if( v == -1 || minE[v] == 1e9) break;

        used[v] = true;
        total += minE[v];

        for(int to = 0; to < n; ++to)
        {
            int w = g.getEdge(v, to);
            if(w > 0 && !used[to] && w < minE[to]) minE[to] = w;
        }
    }
    return total;
}

#endif //HW_10_MSTWEIGHT_HPP
