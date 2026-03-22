#include "../inc/Polynimial.hpp"
#include<iostream>

template <typename T>
T Polynomial<T>::evaluate(const std::vector<T>& point) const
{
    if(point.size() != variables.size())
    {
        throw std::invalid_argument("");
    }
    return evalTree(root.get(), 0, variables.size(), point);
}

template <typename T>
int Polynomial<T>::degreeOfHomogeneity() const
{
    auto supp = support();
    if(supp.empty()) return 0;
    int expected_deg = supp[0].totalDegree();
    for(size_t i = 0; i < supp.size(); ++i)
    {
        if(supp[i].totalDegree() != expected_deg) return -1;
    }
    return expected_deg;
}

template <typename T>
std::pair<Polynomial<T>, Polynomial<T>> Polynomial<T>::decomposeHomogeneous(int degree) const
{
    Polynomial<T> h(variables);
    Polynomial<T> g(variables);

    for(const auto& mono : support())
    {
        if(mono.totalDegree() == degree)
        {
            h.addMonomial(mono.powers, mono.coeff);
        }
        else
        {
            g.addMonomial(mono.powers, mono.coeff);
        }
    }
    return{h, g};
}

template <typename T>
std::ostream& operator<<(
        std::ostream&os,
        const Polynomial<T>& poly
        )
{
    auto supp = poly.support();
    if (supp.empty()) return os << "0";

    bool first = true;
    for (const auto& mono : supp) {
        if (!first) os << " + ";
        os << "(" << mono.coeff << ")";
        for (size_t i = 0; i < poly.variables.size(); ++i) {
            if (mono.powers[i] > 0) {
                os << poly.variables[i];
                if (mono.powers[i] > 1) os << "^" << mono.powers[i];
            }
        }
        first = false;
    }
    return os;
}