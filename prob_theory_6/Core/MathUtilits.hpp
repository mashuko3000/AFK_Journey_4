//
// Created by mvideo on 24/03/26.
//

#ifndef PROB_THEORY_6_MATHUTILITS_HPP
#define PROB_THEORY_6_MATHUTILITS_HPP

#include<algorithm>
#include<cmath>

class MathUtilits
{
public:
    static double combination(size_t n, size_t k)
    {
        if(k > n)
        {
            return 0.0;
        }
        if(k ==0 || k == n)
        {
            return 1.0;
        }

        k = std::min(k, n -k);
        double res = 1.0;
        for(size_t i = 1; i <= k; ++i)
        {
            res = res * (n - i + 1) / i;
        }

        return res;
    }

    static double bernoulliProb(size_t n, size_t k, double p)
    {
        return combination(n, k) * std::pow(p, k) * std::pow(1.0 - p, n - k);
    }
};

#endif //PROB_THEORY_6_MATHUTILITS_HPP
