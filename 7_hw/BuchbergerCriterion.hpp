//
// Created by maria on 11.04.2026.
//

#ifndef INC_7_HW_BUCHBERGERCRITERION_HPP
#define INC_7_HW_BUCHBERGERCRITERION_HPP

#include"Polynimial.hpp"
#include"SPolynomial.hpp"
#include"interfaces.h"
#include"PolyDivision.h"

#include<vector>

template<typename T>

class BuchbergerCriterion final
{
public:
    BuchbergerCriterion() = delete;

    [[nodiscard]] static bool verify(
            const std::vector<Polynomial<T>>& basis,
            const MonomialOrder<T>& order)
    {
        validateBasis(basis);

        const size_t basisSize = basis.size();

        for(size_t i = 0; i < basisSize; i++)
        {
            for(size_t j = i + 1; j < basisSize; j++)
            {
                const auto sPoly = SPolynomial<T>::compute(basis[i],
                                                           basis[j],
                                                           order);

                if(sPoly.support().empty())
                {
                    continue;
                }

                const auto divisionRes = Division<T>::divide(sPoly, basis, order);

                if(!divisionRes.remainder.support().empty())
                {
                    return false;
                }
            }
        }

        return true;
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

#endif //INC_7_HW_BUCHBERGERCRITERION_HPP
