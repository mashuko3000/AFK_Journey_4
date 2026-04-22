//
// Created by maria on 22.04.2026.
//

#ifndef HW_10_GRAPH_HPP
#define HW_10_GRAPH_HPP

#include<vector>

struct Graph
{
    int n;
    std::vector<int> adj;

    Graph(int n) : n(n)
    {
        if(n < 0 || n > 20) throw("Count of edge cant be negative or more 20 for this ");
        adj.assign(n * n, 0);
    }

    void setEdge(int i, int j, int value)
    {
        if(i < 0 || i >= n || j < 0 || j >= n) throw ("Invalid params for setting edge");
        adj[i * n + j] = adj[j * n + i] = value;
    }

    int getEdge(int i, int j) const
    {
        if(i < 0 || i >= n || j < 0 || j >= n) throw ("Invalid params for setting edge");
        return adj[i * n + j];
    }

    void clear()
    {
        std::fill(adj.begin(), adj.end(), 0);
    }
};

#endif //HW_10_GRAPH_HPP
