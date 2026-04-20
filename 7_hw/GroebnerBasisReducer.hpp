//
// Created by maria on 11.04.2026.
//

#ifndef INC_7_HW_GROEBNERBASISREDUCER_HPP
#define INC_7_HW_GROEBNERBASISREDUCER_HPP

#include "Polynimial.hpp"
#include "PolyDivision.h"
#include "MinimalBasisTransformer.hpp"
#include"MonomialUtils.hpp"
template<typename T>
class ReducedBasisTransformer
{
public:
    static std::vector<Polynomial<T>> computeReduced(
            const std::vector<Polynomial<T>> minimalGB,
            const MonomialOrder<T>& order)
    {
        std::vector<Polynomial<T>> reducedGB;

        for(size_t i = 0; i < minimalGB.size(); ++i)
        {
            Polynomial<T> p = minimalGB[i];
            std::vector<Polynomial<T>> others;
            for(size_t j = 0; j < minimalGB.size(); ++j)
            {
                if(i!=j)
                {
                    others.push_back(minimalGB[j]);
                }
            }
            if (others.empty())
            {
                if (!p.support().empty())
                    makeMonic(p, order);
                reducedGB.push_back(std::move(p));
            }
            else
            {
                auto divisionRes = Division<T>::divide(p, others, order);
                Polynomial<T> remainder = divisionRes.remainder;

                if (!remainder.support().empty())
                    makeMonic(remainder, order);

                reducedGB.push_back(std::move(remainder));
            }
        }
        return reducedGB;
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
};

#endif //INC_7_HW_GROEBNERBASISREDUCER_HPP
