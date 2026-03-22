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

    int totalDegree() const
    {
        int sum = 0;
        for(int p : powers) sum += p;
        return sum;
    }
};

#endif //INC_4_HW_MONOMIAL_HPP
