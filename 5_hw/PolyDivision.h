//
// Created by maria on 28.03.2026.
//

#ifndef INC_5_HW_POLYDIVISION_H
#define INC_5_HW_POLYDIVISION_H

#include "../4_hw/inc/Polynimial.hpp"
#include "interfaces.h"
#include <vector>
#include <stdexcept>

template<typename T>
struct DivisionResult
{
    std::vector<Polynomial<T>> quotients;
    Polynomial<T> remainder;
};

template<typename T>
class Division
{
public:
    [[nodiscard]] static DivisionResult<T> divide(
            const Polynomial<T>& dividend,
            const std::vector<Polynomial<T>>& divisors,
            const MonomialOrder<T>& order
            )
    {
        validateInput(dividend, divisors);

        const size_t s = divisors.size();
        const auto& vars = dividend.getVariables();

        DivisionResult<T> result{std::vector<Polynomial<T>>(s, Polynomial<T>(vars)),
                                 Polynomial<T>(vars)};

        Polynomial<T> p = dividend;

        while(!p.support().empty())
        {
            bool divisionOccurred = false;

            const Monomial<T> p_lt = p.lt(order);

            for(size_t i = 0; i < s; ++i)
            {
                if (divisors[i].support().empty())
                {
                    continue;
                }

                const Monomial<T> fi_lt = divisors[i].lt(order);

                if(divides(fi_lt.powers, p_lt.powers))
                {
                    const auto term = p_lt/fi_lt;

                    Polynomial<T>termPoly(vars);
                    termPoly.addMonomial(term.powers, term.coeff);

                    result.quotients[i] = result.quotients[i] + termPoly;
                    p = p - (termPoly * divisors[i]);

                    divisionOccurred = true;
                    break;
                }
            }

            if(!divisionOccurred)
            {
                result.remainder.addMonomial(p_lt.powers, p_lt.coeff);

                Polynomial<T> remTermPoly(vars);
                remTermPoly.addMonomial(p_lt.powers, p_lt.coeff);

                p = p - remTermPoly;
            }
        }
        return result;
    }

private:
    static void validateInput(
            const Polynomial<T>& dividend,
            const std::vector<Polynomial<T>>& divisors)
    {
        if (divisors.empty())
        {
            throw std::invalid_argument("Divisors set cannot be empty.");
        }

        const auto& vars = dividend.getVariables();
        for (const auto& div : divisors)
        {
            if (div.getVariables() != vars)
            {
                throw std::invalid_argument("All polynomials must have the same variables space.");
            }
        }
    }

    [[nodiscard]] static bool divides(
            const std::vector<int>& divisorPowers,
            const std::vector<int>& dividendPowers)
    {
        if (divisorPowers.size() != dividendPowers.size())
        {
            throw std::invalid_argument("Dimension mismatch in monomial division check.");
        }
        for (size_t i = 0; i < divisorPowers.size(); ++i)
        {
            if (divisorPowers[i] > dividendPowers[i])
            {
                return false;
            }
        }
        return true;
    }
};

#endif //INC_5_HW_POLYDIVISION_H
