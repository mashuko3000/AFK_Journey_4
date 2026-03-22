#include "../inc/Polynimial.hpp"

template <typename T>
Polynomial<T> Polynomial<T>::operator+(
        const Polynomial<T>& other
) const
{
    Polynomial<T> result(variables);
    result.root = addTrie(this->root.get(), other.root.get(), 0, variables.size());
    cleanTree(result.root.get(), 0, variables.size());
    return result;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator-(
        const Polynomial<T>& other
) const
{
    Polynomial result(variables);
    auto negOther = negateTree(
            other.root.get(),
            0,
            variables.size()
            );
    result = *this + other;
    return result;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator*(
        const Polynomial<T>& other
) const
{
    Polynomial result(variables);
    if(!root || !other.root) return result;

    auto supp_this = this->support();
    auto supp_other= other.support();

    const auto& small_supp = (supp_this.size() <= supp_other.size()) ? supp_this : supp_other;
    const Polynomial<T>& large_poly = (supp_this.size() <= supp_other.size()) ? other : *this;

    for (const auto& mono : small_supp)
    {
        if (mono.coeff == T(0)) continue;
        auto product_root = mulTrieByMonomials(
                large_poly.root.get(),
                mono.powers,
                mono.coeff,
                0,
                variables.size()
        );

        if (product_root)
        {
            result.root = addTrie(result.root.get(), product_root.get(), 0, variables.size());
        }
    }

    cleanTree(result.root.get(), 0, variables.size());
    return result;
}