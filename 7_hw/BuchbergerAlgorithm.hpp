//
// Created by maria on 11.04.2026.
//

#ifndef INC_7_HW_BUCHBERGERALGORITHM_HPP
#define INC_7_HW_BUCHBERGERALGORITHM_HPP

#include"Polynimial.hpp"
#include "SPolynomial.hpp"
#include "PolyDivision.h"
#include "interfaces.h"
#include"MinimalBasisTransformer.hpp"

#include<deque>

template<typename T>
class BuchbergerAlgorithm final
{
public:
    BuchbergerAlgorithm() = delete;

    [[nodiscard]] static std::vector<Polynomial<T>> computeMinimal(const std::vector<Polynomial<T>>& initialBasis,
                                                            const MonomialOrder<T>& order)
    {
        validateBasis(initialBasis);

        std::vector<Polynomial<T>> groebnerBasis = initialBasis;
        std::deque<std::pair<size_t, size_t>> paris;

        for (size_t i = 0; i < groebnerBasis.size(); ++i)
        {
            for(size_t j = i + 1; j < groebnerBasis.size(); ++j)
            {
                paris.emplace_back(i, j);
            }
        }

        while(!paris.empty())
        {
            auto[i, j] = paris.front();
            paris.pop_front();

            auto SPoly = SPolynomial<T>::compute(groebnerBasis[i], groebnerBasis[j], order);
            if(SPoly.support().empty()) continue;

            auto divisionRes = Division<T>::divide(SPoly, groebnerBasis, order);
            const auto& remainder = divisionRes.remainder;

            if (!remainder.support().empty())
            {
                size_t newIdx = groebnerBasis.size();
                for (size_t k = 0; k < newIdx; ++k)
                {
                    paris.emplace_back(k, newIdx);
                }
                groebnerBasis.push_back(remainder);
            }
        }
        return MinimalBasisTransformer<T>::transform(std::move(groebnerBasis), order);
    }

private:
    static void validateBasis(const std::vector<Polynomial<T>>& basis)
    {
        if (basis.empty())
        {
            throw std::invalid_argument("Basis collection cannot be empty.");
        }

        const auto& expextedVariables = basis.front().getVariables();

        for(const auto& m : basis)
        {
            const auto& currentVariables = m.getVariables();

            if(currentVariables != expextedVariables)
            {
                throw std::invalid_argument("All polynomials in the basis must share the same variable space.");
            }
            if (m.support().empty())
            {
                throw std::invalid_argument("Basis collection cannot contain zero polynomials.");
            }
        }
    }
};

#endif //INC_7_HW_BUCHBERGERALGORITHM_HPP
