//
// Created by mvideo on 21/03/26.
//

#ifndef INC_4_HW_TRIENODE_HPP
#define INC_4_HW_TRIENODE_HPP

#include<memory>
#include<map>

template<typename T>
struct TrieNode
{
    std::map<int, std::unique_ptr<TrieNode<T>>> children; // pow and ptr
    T coeff;

    TrieNode() : coeff(T(0)) {};

    std::unique_ptr<TrieNode<T>> clone() const
    {
        auto res = std::make_unique<TrieNode<T>>();
        res->coeff = coeff;
        for(auto& [p, c] : children)
        {
            res->children[p] = c->clone();
        }
        return res;
    }
};

#endif //INC_4_HW_TRIENODE_HPP
