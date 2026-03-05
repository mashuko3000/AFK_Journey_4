
#include "tree_model.hpp"

node* tree_model::build_recursive(int current_level,
                      float L,
                      float R,
                      i_distribution_model* proto,
                      double p_stay)
{
    bool is_leaf = (current_level >= m_height);
    node* current = new node(is_leaf ? nullptr : proto,
                             p_stay,
                             is_leaf,
                             0.0f,
                             0.0f,
                             current_level);
    current -> x = (L + R) / 2.0f;
    current-> y = m_height > 0 ? static_cast<float>(current_level) / m_height : 0.0f;
    if(!is_leaf)
    {
        float slot_width = (R - L) / static_cast<float>(m_M);
        for (size_t i = 0; i < m_M; ++i)
        {
            float child_L = L + i * slot_width;
            float child_R = L + (i + 1) * slot_width;

            try
            {
                current->children.push_back(
                        build_recursive(current_level + 1, child_L, child_R, proto, p_stay)
                );
            }
            catch (...)
            {
                delete current;
                throw;
            }
        }
    }
    return current;
}

tree_model::tree_model(int M, int H, i_distribution_model* model_proto, double p_stay) : m_root(nullptr), m_height(H), m_M(M)
{
    if (H < 0) throw std::invalid_argument("tree height cannot be negative");
    if (M <= 0) throw std::invalid_argument("M must be at least 1");
    if (!model_proto && H > 0) throw std::invalid_argument("distribution model is required for non-zero height");
    m_root = build_recursive(0, 0.0f, 1.0f, model_proto, p_stay);
}
tree_model::~tree_model()
{
    delete m_root;
    m_root = nullptr;
}

tree_model::tree_model(tree_model&& other) noexcept : m_root(other.m_root), m_height(other.m_height), m_M(other.m_M)
{
    other.m_root = nullptr;
    other.m_height = 0;
    other.m_M = 0;
}
tree_model& tree_model::operator=(tree_model&& other) noexcept
{
    if (this != &other)
    {
        delete m_root;

        m_root = other.m_root;
        m_height = other.m_height;
        m_M = other.m_M;

        other.m_root = nullptr;
        other.m_height = 0;
        other.m_M = 0;
    }
    return *this;
}

void tree_model::reset_all_stats()
{
    if (m_root) m_root->reset_stats();
}

void tree_model::collect_leaves(node* current, std::vector<node*>& leaves) {
    if (!current) return;
    if (current->is_leaf) {
        leaves.push_back(current);
        return;
    }
    for (auto child : current->children) {
        collect_leaves(child, leaves);
    }
}