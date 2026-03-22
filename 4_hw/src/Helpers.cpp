#include "../inc/Polynimial.hpp"

template<typename T>
std::unique_ptr<TrieNode<T>> Polynomial<T>::negateTree(
        const TrieNode<T>* node,
        int depth,
        int max_depth)
{
    if(!node)return nullptr;
    auto res = std::make_unique<TrieNode<T>>();
    if(depth == max_depth)
    {
        res->coeff = T(0) - node->coeff;
        return res;
    }
    for(const auto&[power, child] : node->children)
    {
        res->children[power] = negateTree(child.get(), depth+1, max_depth);
    }
    return res;
}

template<typename T>
std::unique_ptr<TrieNode<T>> Polynomial<T>::mulTrieByMonomials(
        const TrieNode<T>* node,
        const std::vector<int>& p,
        T c,
        int depth,
        int max_depth
)
{
    if(!node) return nullptr;
    auto res = std::make_unique<TrieNode<T>>();

    if(depth == max_depth)
    {
        res->coeff = node->coeff * c;
        return res;
    }

    for(const auto& [power, child] : node->children)
    {
        res->children[power + p[depth]] = mulTrieByMonomials(child.get(), p, c, depth + 1, max_depth);
    }

    return res;
}

template<typename T>
void Polynomial<T>::collectSup(
        const TrieNode<T>* node,
        int depth,
        int max_depth,
        std::vector<int>& current_powers,
        std::vector<Monomial<T>>& result
)
{
    if(!node) return;
    if(depth == max_depth)
    {
        if(node->coeff != T(0)) result.push_back({current_powers, node->coeff});
        return;
    }
    for(const auto& [power, child] : node->children)
    {
        current_powers[depth] = power;
        collectSup(child.get(), depth + 1, max_depth, current_powers, result);
    }
}

template <typename T>
bool Polynomial<T>::isEqual(
        const TrieNode<T>* a,
        const TrieNode<T>* b,
        int depth,
        int max_depth
)
{
    if(!a && ! b) return true;
    if(!a || !b) return false;

    if(depth == max_depth) return a->coeff == b->coeff;
    if(a->children.size() != b->children.size()) return false;
    auto itA = a->children.begin();
    auto itB = b->children.begin();

    while(itA != a->children.end())
    {
        if(itA->first != itB->first) return false;
        if(!isEqual(itA->second.get(), itB->second.get(), depth + 1, max_depth)) return false;
        ++itA;
        ++itB;
    }
    return true;
}

template <typename T>
T Polynomial<T>::evalTree(
        const TrieNode<T>* node,
        int depth,
        int max_depth,
        const std::vector<T>& point
)
{
    if(!node) return T(0);
    if(depth == max_depth) return node->coeff;

    T sum = T(0);
    for(const auto& [power, child] : node->children)
    {
        T term = fastPower(point[depth], power);
        sum += term * evalTree(child.get(), depth + 1, max_depth, point);
    }

    return sum;
}

