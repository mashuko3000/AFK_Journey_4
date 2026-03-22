#include "../inc/Polynimial.hpp"

template <typename T>
void Polynomial<T>::addMonomial(const std::vector<int>& powers, T coeff)
{
    if(powers.size() != variables.size()) throw std::invalid_argument("Invalid count of powers");
    if(coeff == T(0)) return;

    TrieNode<T>* current = root.get();
    for(int p : powers)
    {
        if(current->children.find(p) == current->children.end())
        {
            current->children[p] = std::make_unique<TrieNode<T>>();
        }
        current = current->children[p].get();
    }
    current->coeff += coeff;
    cleanTree(root.get(), 0, variables.size());
}

template <typename T>
std::unique_ptr<TrieNode<T>> Polynomial<T>::addTrie(
        const TrieNode<T>* a,
        const TrieNode<T>* b,
        int depth,
        int max_depth
)
{
    if(!a && !b) return nullptr;
    if(!a) return b->clone();
    if(!b) return a->clone();

    auto res = std::make_unique<TrieNode<T>>();

    if(depth == max_depth)
    {
        res->coeff = a->coeff + b->coeff;
        return res;
    }

    auto itA = a->children.begin();
    auto itB = b->children.begin();

    while(itA != a->children.end() && itB != b->children.end())
    {
        if(itA->first < itB->first)
        {
            res->children[itA->first] = itA->second->clone();
            ++itA;
        }
        else if(itA->first > itB->first)
        {
            res->children[itB->first] = itB->second->clone();
            ++itB;
        }
        else
        {
            auto merged = addTrie(itA->second.get(), itB->second.get(), depth +1, max_depth);
            if(merged) res->children[itA->first] = std::move(merged);
            ++itA;
            ++itB;
        }
    }

    while(itA != a->children.end())
    {
        res->children[itA->first] = itA->second->clone();
        ++itA;
    }

    while(itB != b->children.end())
    {
        res->children[itB->first] = itB->second->clone();
        ++itB;
    }
    return res;
}