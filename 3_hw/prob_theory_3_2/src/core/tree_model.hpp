//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_2_TREE_MODEL_HPP
#define PROB_THEORY_3_2_TREE_MODEL_HPP

#include"node.hpp"

class tree_model
{
private:
    node* m_root;
    int m_height;
    size_t m_M;

    node* build_recursive(int current_level,
                          float L,
                          float R,
                          i_distribution_model* proto,
                          double p_stay);

public:
    tree_model(int M, int H, i_distribution_model* model_proto, double p_stay);
    ~tree_model();
    tree_model(const tree_model& other) = delete;
    tree_model& operator=(const tree_model& other) = delete;
    tree_model(tree_model&& other) noexcept;
    tree_model& operator=(tree_model&& other) noexcept;

    node* get_root() const {return m_root;}
    int get_height() const {return m_height;}
    size_t get_M() const  {return m_M;}

    void collect_leaves(node* current, std::vector<node*>& leaves);

    void reset_all_stats();
};


#endif //PROB_THEORY_3_2_TREE_MODEL_HPP
