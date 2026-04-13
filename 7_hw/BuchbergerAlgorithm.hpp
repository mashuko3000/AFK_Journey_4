//
// Created by maria on 11.04.2026.
//

#ifndef INC_7_HW_BUCHBERGERALGORITHM_HPP
#define INC_7_HW_BUCHBERGERALGORITHM_HPP

#include"Polynimial.hpp"
#include "SPolynomial.hpp"
#include "PolyDivision.h"
#include "interfaces.h"

template<typename T>
class BuchbergerAlgorithm final
{
public:
    BuchbergerAlgorithm() = delete;

    [[nodiscard]] static std::vector<Polynomial<T>> compute(const std::vector<Polynomial<T>>& initialBasis,
                                                            const MonomialOrder<T> order)
    {
        validateBasis(initialBasis);

        std::vector<Polynomial<T>> groebnerBasis = initialBasis;
        bool basisExpanded = true;

        while(basisExpanded)
        {
            basisExpanded = false;
            const size_t currentSize = groebnerBasis.size();

            for(size_t i = 0; i < currentSize; ++i)
            {
                for(size_t j = i + 1; j < currentSize; ++j)
                {
                    const auto sPoly = SPolynomial<T>::compute(groebnerBasis[i],
                                                               groebnerBasis[j],
                                                               order);

                    if(sPoly.support().empty())
                    {
                        continue;
                    }

                    const auto divisionRes = Division<T>::divide(sPoly, groebnerBasis, order);

                    if(!divisionRes.remainder.support().empty())
                    {
                        groebnerBasis.push_back(divisionRes.remainder);
                        basisExpanded = true;
                    }
                }
            }
        }
        return groebnerBasis;
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
