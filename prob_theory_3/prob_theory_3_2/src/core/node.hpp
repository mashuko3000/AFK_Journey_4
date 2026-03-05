//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_2_NODE_HPP
#define PROB_THEORY_3_2_NODE_HPP

#include"../math/idistribution.hpp"
#include"../math/distributions.hpp"
#include<vector>

struct node {
    std::vector<node*> children;
    i_distribution_model* dist_model;
    bool is_leaf;

    double p_stay;

    unsigned long long visit_count = 0;
    unsigned long long stay_count = 0;

    float x, y;
    int level;

    node(i_distribution_model* model = nullptr,
         double p = 0.0,
         bool leaf = false,
         float nx = 0.0f,
         float ny = 0.0f,
         int lvl = 0) :
         p_stay(p),
         is_leaf(leaf),
         x(nx),
         y(ny),
         level(lvl)
    {
        if (p_stay < 0.0 || p_stay > 1.0)
        {
            throw std::invalid_argument("node: p_stay must be in range [0.0 .. 1.0]");
        }
        dist_model = model ? model->clone() : nullptr;
    }

    ~node() noexcept
    {
        delete dist_model;
        for (auto child : children) delete child;
    }

    node(const node&) = delete;
    node& operator=(const node&) = delete;
    node(node&&) = delete;
    node& operator=(const node&&) = delete;

    void reset_stats()
    {
        visit_count = 0;
        stay_count = 0;
        for (auto child: children) child->reset_stats();
    }

    bool isLeaf() const noexcept {return is_leaf;}
    double get_stay_probability() const noexcept {return p_stay;}
    int get_level() const noexcept {return level;}
    size_t child_count() const noexcept {return children.size();}
};


#endif //PROB_THEORY_3_2_NODE_HPP
