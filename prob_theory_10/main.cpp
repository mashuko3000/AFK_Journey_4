#include <iostream>

#include "Graph.hpp"
#include "MSTWeight.hpp"
#include "IsConnected.hpp"
#include "LongestCycle.hpp"
#include "Stat.hpp"
#include "Analize.hpp"

#include <random>
#include <iomanip>

int main() {
    int n = 10;
    int iters = 10000;
    double p = 0.2;

    Graph g(n);
    std::mt19937 rng(std::random_device{}());
    std::bernoulli_distribution edge_dist(p);
    std::uniform_int_distribution<int> weight_dist(1, 10);

    Stat stA, stB, stC, stD, stE, stF, stG;

    std::cout << "Start" << std::endl;

    for (int i = 0; i < iters; ++i)
    {
        g.clear();
        for (int u = 0; u < n; ++u)
        {
            for (int v = u + 1; v < n; ++v)
            {
                if (edge_dist(rng)) g.setEdge(u, v, weight_dist(rng));
            }
        }

        auto components = analyzeComponents(g);

        int isolated = 0;
        int trees = 0;
        int cliques = 0;

        for (const auto& c : components)
        {
            if (c.vCount == 1) isolated++;
            else
            {
                if (c.isTree) trees++;
                if (c.isClique) cliques++;
            }
        }
        stD.add(isolated);
        stE.add(trees);
        stF.add(components.size());
        stG.add(cliques);

        if (isConnected(g))
        {
            stA.add(getMSTWeight(g));

            int max_e = 0;
            int max_w = 0;

            for(int start_node = 0; start_node < n; ++start_node)
            {
                findLongestCycle(start_node, start_node, (1 << start_node), 0, 0, max_e, max_w, g);
            }

            stB.add(max_e);
            stC.add(max_w);
        }
    }

    auto print_line = [](std::string label, const Stat& s)
    {
        std::cout << std::left << std::setw(30) << label
                  << " | E = " << std::fixed << std::setprecision(4) << std::setw(8) << s.getMean()
                  << " | Var = " << std::setw(8) << s.getVariance()
                  << " | n=" << s.count << "\n";
    };

    std::cout << std::string(75, '-') << "\n";
    std::cout << "Feature                     | M | D | Iter\n";
    std::cout << std::string(75, '-') << "\n";
    print_line("A: Weight(connected)", stA);
    print_line("B: Max cycle(edges)", stB);
    print_line("C: Max cycle(weight)", stC);
    print_line("D: Isolated vertices", stD);
    print_line("E: Tree components (>1)", stE);
    print_line("F: Number of comp", stF);
    print_line("G: Clique components (>1)", stG);
    std::cout << std::string(75, '-') << "\n";

    return 0;
}