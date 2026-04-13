//
// Created by maria on 11.04.2026.
//

#ifndef INC_7_HW_SPOLYNOMIAL_HPP
#define INC_7_HW_SPOLYNOMIAL_HPP

#include"Polynimial.hpp"
#include "interfaces.h"
#include"MonomialUtils.hpp"

template<typename T>
class SPolynomial final {
public:
    SPolynomial() = delete;

    [[nodiscard]] static Polynomial<T> compute(const Polynomial<T> &f,
                                               const Polynomial<T> &g,
                                               const MonomialOrder<T> &order) {
        validatePolynomials(f, g);

        const auto LTf = f.lt(order);
        const auto LTg = g.lt(order);

        const auto LCMPowers = MonomialUtils<T>::LCM(LTf.powers, LTg.powers);
        const Monomial<T> LCMMonomial{LCMPowers, T(1)};

        const Monomial<T> multiplierF = LCMMonomial / LTf;
        const Monomial<T> multiplierG = LCMMonomial / LTg;

        const auto variables = f.getVariables();

        const auto polyMultiplierF = MonomialUtils<T>::createPolynomialFromTerm(multiplierF.powers,
                                                                                multiplierF.coeff,
                                                                                variables);

        const auto polyMultiplierG = MonomialUtils<T>::createPolynomialFromTerm(multiplierG.powers,
                                                                                multiplierG.coeff,
                                                                                variables);


        return (polyMultiplierF * f) - (polyMultiplierG * g);
    }

private:
    static void validatePolynomials(
            const Polynomial<T> &f,
            const Polynomial<T> &g) {
        if (f.getVariables() != g.getVariables()) {
            throw std::invalid_argument("Polynomials must have identical variable spaces to compute S-polynomial.");
        }

        if (f.support().empty() || g.support().empty()) {
            throw std::invalid_argument("Cannot compute S-polynomial for zero polynomials.");
        }
    }
};

#endif //INC_7_HW_SPOLYNOMIAL_HPP
