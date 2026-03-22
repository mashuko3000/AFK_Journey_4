#include "../inc/Polynimial.hpp"

template <typename T>

bool Polynomial<T>::cleanTree(
        TrieNode<T>* node,
        int depth,
        int max_depth
)
{
    if(!node) return true;
    if(depth == max_depth) return node->coeff == T(0);

    for(auto it = node->children.begin(); it!=node->children.end();)
    {
        if(cleanTree(it->second.get(), depth+1, max_depth))
        {
            it = node->children.erase(it);
        }
        else
        {
            ++it;
        }
    }
    return node->children.empty();
}