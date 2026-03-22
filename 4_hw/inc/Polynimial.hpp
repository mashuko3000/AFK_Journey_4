//
// Created by mvideo on 21/03/26.
//

#ifndef INC_4_HW_POLYNIMIAL_HPP
#define INC_4_HW_POLYNIMIAL_HPP

#include"Monomial.hpp"
#include"TrieNode.hpp"

#include<stdexcept>

template<typename T>
class Polynomial final
{
private: //fields
    std::unique_ptr<TrieNode<T>> root;
    std::vector<std::string> variables;

public:
    explicit Polynomial(
            const std::vector<std::string>& vars
            )
            :
            variables(vars),
            root(std::make_unique<TrieNode<T>>())
    {
        if(vars.empty()) throw std::invalid_argument("Should be at least 1 var");
    }

    ~Polynomial() = default;

    Polynomial(
            const Polynomial& other)
            :
            variables(other.variables),
            root(other.root ? other.root->clone() : nullptr){}

    Polynomial& operator=(const Polynomial& other);
    void addMonomial(const std::vector<int>& powers, T coeff);

    Polynomial operator+(
            const Polynomial& other
            ) const;
    Polynomial operator-(
            const Polynomial& other
    ) const;
    Polynomial operator*(
            const Polynomial& other
    ) const;

    std::vector<Monomial<T>> support() const
    {
        std::vector<Monomial<T>> result;
        std::vector<int> currentPowers(variables.size(), 0);
        collectSup(root.get(), 0, variables.size(), currentPowers, result);
        return result;
    }

    bool operator==(
            const Polynomial<T>& other
            ) const
    {
        if(variables != other.variables) return false;
        return isEqual(root.get(), other.root.get(), 0, variables.size());
    }

    bool operator!=(const Polynomial<T>& other) const
    {
        return !(*this == other);
    }

    T evaluate(const std::vector<T>& point) const;
    int degreeOfHomogeneity() const;
    std::pair<Polynomial<T>, Polynomial<T>> decomposeHomogeneous(int degree) const;
    template <typename U>
    friend std::ostream& operator<<(
            std::ostream&os,
            const Polynomial<U>& poly
            );

private:
    //clean empty threads
    static bool cleanTree(
            TrieNode<T>* node,
            int depth,
            int max_depth
            );
    static std::unique_ptr<TrieNode<T>> addTrie(
            const TrieNode<T>* a,
            const TrieNode<T>* b,
            int depth,
            int max_depth
            );

    static std::unique_ptr<TrieNode<T>> negateTree(
            const TrieNode<T>* node,
            int depth,
            int max_depth
            );
    static std::unique_ptr<TrieNode<T>> mulTrieByMonomials(
            const TrieNode<T>* node,
            const std::vector<int>& p,
            T c,
            int depth,
            int max_depth
            );

    static void collectSup(
            const TrieNode<T>* node,
            int depth,
            int max_depth,
            std::vector<int>& current_powers,
            std::vector<Monomial<T>>& result
            );

    static bool isEqual(
            const TrieNode<T>* a,
            const TrieNode<T>* b,
            int depth,
            int max_depth
            );

    static T evalTree(
            const TrieNode<T>* node,
            int depth,
            int max_depth,
            const std::vector<T>& point
            );

    static T fastPower(
            T base,
            int exp
            )
    {
        if(exp < 0)
        {
            throw std::invalid_argument("Negative exponent not supported for this ring");
        }
        T res = T(1);
        while(exp > 0)
        {
            if(exp&1) res = res * base;
            base = base * base;
            exp >>= 1;
        }
        return res;
    }
};

#include "../src/addHelpers.cpp"
#include "../src/arithmeticOp.cpp"
#include "../src/cleanHelper.cpp"
#include "../src/Helpers.cpp"
#include "../src/Other.cpp"
#include "../src/Polynomial.cpp"

#endif //INC_4_HW_POLYNIMIAL_HPP
