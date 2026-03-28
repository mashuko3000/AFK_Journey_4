//
// Created by mvideo on 21/03/26.
//

#ifndef INC_4_HW_MONOMIAL_HPP
#define INC_4_HW_MONOMIAL_HPP

#include<vector>

template<typename T>
struct Monomial
{
    std::vector<int> powers;
    T coeff;

    [[nodiscard]]int totalDegree() const
    {
        int sum = 0;
        for(int p : powers) sum += p;
        return sum;
    }

    [[nodiscard]] std::string toString(const std::vector<std::string>& vars) const
    {
        if (coeff == 0) return "0";

        std::string res = std::to_string(coeff);
        for (size_t i = 0; i < powers.size(); ++i)
        {
            if (powers[i] == 0) continue;
            res += vars[i];
            if (powers[i] > 1)
            {
                res += "^" + std::to_string(powers[i]);
            }
        }
        return res;
    }

    Monomial<T>& operator /=(const Monomial<T> other)
    {
        if(other.coeff == T(0))
        {
            throw std::runtime_error("Division by zero coefficient in Monomial.");
        }
        this->coeff = this->coeff / other.coeff;

        for(size_t i = 0; i < this->powers.size(); ++i)
        {
            this->powers[i] -= other.powers[i];
        }
        return *this;
    }

    [[nodiscard]] Monomial<T> operator /(const Monomial<T> other) const
    {
        Monomial<T> result = *this;
        result /= other;
        return result;
    }
};

#endif //INC_4_HW_MONOMIAL_HPP
