//
// Created by maria on 22.04.2026.
//

#ifndef HW_10_ISCONNECTED_HPP
#define HW_10_ISCONNECTED_HPP

#include "Graph.hpp"

bool isConnected(const Graph& g)
{
    if (g.n <= 1) return true;

    std::vector<bool> visited(g.n, false);
    std::vector<int> q;

    q.reserve(g.n);
    q.push_back(0);

    visited[0] = true;

    int head = 0;
    while(head < q.size())
    {
        int u = q[head++];
        for(int v = 0; v < g.n; ++v)
        {
            if(g.getEdge(u, v) > 0 && !visited[v])
            {
                visited[v] = true;
                q.push_back(v);
            }
        }
    }
    return q.size() == g.n;
}

#endif //HW_10_ISCONNECTED_HPP
