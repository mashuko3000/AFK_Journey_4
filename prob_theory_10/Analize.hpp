//
// Created by maria on 22.04.2026.
//

#ifndef HW_10_ANALIZE_HPP
#define HW_10_ANALIZE_HPP

#include "Graph.hpp"

struct CompInfo
{
    int vCount;
    bool isTree;
    bool isClique;
};

std::vector<CompInfo> analyzeComponents(const Graph& g)
{
    std::vector<CompInfo> res;
    std::vector<bool> visited(g.n, false);

    for (int i = 0; i < g.n; ++i)
    {
        if (!visited[i])
        {
            std::vector<int> nodes;
            std::vector<int> q = {i};

            visited[i] = true;
            int head = 0;

            while(head < q.size())
            {
                int u = q[head++];
                nodes.push_back(u);
                for(int v = 0; v < g.n; ++v)
                {
                    if (g.getEdge(u, v) > 0 && !visited[v])
                    {
                        visited[v] = true;
                        q.push_back(v);
                    }
                }
            }

            int edge_count = 0;
            for(size_t uIdx = 0; uIdx < nodes.size(); ++uIdx)
            {
                for(size_t vIdx = uIdx + 1; vIdx < nodes.size(); ++vIdx)
                {
                    if(g.getEdge(nodes[uIdx], nodes[vIdx]) > 0) edge_count++;
                }
            }

            int v = nodes.size();
            res.push_back({v, (v > 1 && edge_count == v - 1), (v > 1 && edge_count == v * (v - 1) / 2)});
        }
    }
    return res;
}


#endif //HW_10_ANALIZE_HPP
