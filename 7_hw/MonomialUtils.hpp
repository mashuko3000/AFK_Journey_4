//
// Created by maria on 11.04.2026.
//

#ifndef INC_7_HW_MONOMIALUTILS_HPP
#define INC_7_HW_MONOMIALUTILS_HPP

#include<vector>

#include"Polynimial.hpp"

template<typename T>
class MonomialUtils final
{
public:
    MonomialUtils() = delete;

    [[nodiscard]] static std::vector<int> LCM(std::vector<int> APowers,
                                       std::vector<int> BPowers)
    {
        if(APowers.empty() || BPowers.empty() || APowers.size() != BPowers.size())
        {
            throw std::invalid_argument("Monomial dimensions mismatch during LCM computation.");
        }

        const size_t dimension = APowers.size();

        std::vector<int> LCM(dimension);
        for(int i = 0; i < dimension; ++i)
        {
            LCM[i] = std::max(APowers[i], BPowers[i]);
        }

        return LCM;
    }

    [[nodiscard]] static Polynomial<T> createPolynomialFromTerm(
            const std::vector<int>& powers,
            const T& coeff,
            const std::vector<std::string>& variables
            )
    {
        if (variables.empty())
        {
            throw std::invalid_argument("Variables set cannot be empty.");
        }

        if (powers.size() != variables.size())
        {
            throw std::invalid_argument("Monomial powers dimension mismatch with variables count.");
        }

        Polynomial<T> poly(variables);
        poly.addMonomial(powers, coeff);

        return poly;
    }

};

#endif //INC_7_HW_MONOMIALUTILS_HPP
