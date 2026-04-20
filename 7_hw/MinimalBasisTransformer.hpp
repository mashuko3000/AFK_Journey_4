//
// Created by maria on 17.04.2026.
//

#ifndef INC_7_HW_MINIMALBASISTRANSFORMER_HPP
#define INC_7_HW_MINIMALBASISTRANSFORMER_HPP

#include "Polynimial.hpp"
#include "interfaces.h"
#include <vector>
#include <algorithm>
#include<complex>

template<typename T>
class MinimalBasisTransformer final
{
public:
    MinimalBasisTransformer() = delete;

    [[nodiscard]] static std::vector<Polynomial<T>> transform(
            std::vector<Polynomial<T>> basis,
            const MonomialOrder<T>& order)
    {
        if (basis.empty()) return {};

        for (auto& poly : basis)
        {
            makeMonic(poly, order);
        }

        std::sort(basis.begin(), basis.end(), [&](const auto& a, const auto& b)
        {
            return totalDegree(a.lm(order)) < totalDegree(b.lm(order));
        });

        std::vector<Polynomial<T>> minimal;
        minimal.reserve(basis.size());

        for (size_t i = 0; i < basis.size(); ++i)
        {
            bool isRedundant = false;
            const auto& lt_i = basis[i].lm(order);

            for (size_t j = 0; j < basis.size(); ++j)
            {
                if (i == j) continue;
                if (isDivisible(lt_i, basis[j].lm(order)))
                {
                    if (lt_i != basis[j].lm(order) || j < i) {
                        isRedundant = true;
                        break;
                    }
                }
            }

            if (!isRedundant)
            {
                minimal.push_back(std::move(basis[i]));
            }
        }

        return minimal;
    }

private:
    static void makeMonic(Polynomial<T>& poly, const MonomialOrder<T>& order)
    {
        if (poly.support().empty()) return;
        T lc = poly.lc(order);

        T one(1);
        if (!(lc == one))
        {
            T inv = lc.inverse();
            std::vector<int> zeroPowers(poly.getVariables().size(), 0);

            Polynomial<T> scalarPoly = MonomialUtils<T>::createPolynomialFromTerm(
                    zeroPowers,
                    inv,
                    poly.getVariables()
            );

            poly = poly * scalarPoly;
        }
    }

    static bool isDivisible(const std::vector<int>& dividend, const std::vector<int>& divisor)
    {
        for (size_t i = 0; i < dividend.size(); ++i)
        {
            if (dividend[i] < divisor[i]) return false;
        }
        return true;
    }

    static int totalDegree(const std::vector<int>& powers)
    {
        int sum = 0;
        for (int p : powers) sum += p;
        return sum;
    }
};

#endif //INC_7_HW_MINIMALBASISTRANSFORMER_HPP
