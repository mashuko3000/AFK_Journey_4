//
// Created by maria on 31.03.2026.
//

#include "wiener_attack.h"

std::pair<bigint, bigint> WienerAttack::execute(const bigint& n, const bigint& e) const
{
    if(n <= 0 || e <= 0)
    {
        throw std::invalid_argument("Modulus n and exponent e must be strictly positive.");
    }

    if(e >= n)
    {
        throw std::invalid_argument("Exponent e must be less than modulus n.");
    }

    Fraction frac{e, n};

    const auto convergents = fractionService.getConvergents(frac);

    for(const auto& c : convergents)
    {
        const bigint k = c.numerator;
        const bigint d = c.denominator;

        if(k == 0 || d ==0) continue;

        const bigint ed = e*d-1;

        if(ed % k != 0) continue;

        const bigint phi = ed / k;

        const bigint pq = n - phi + 1;
        if(pq <= 0) continue;

        bigint p, q;
        if(check(pq, n, p, q))
        {
            return {std::min(p, q), std::max(p, q)};
        }
    }
    throw std::runtime_error("Wiener's attack failed: Valid private key not found in convergents.");
}

bool WienerAttack::check(const bigint& sum, const bigint& n, bigint& p, bigint& q) const noexcept
{
    const bigint D = sum * sum - 4 * n;
    if( D < 0) return false;

    bigint rootD = boost::multiprecision::sqrt(D);
    if(rootD * rootD != D) return false;

    if((sum + rootD) % 2 != 0) return false;

    p = (sum + rootD) / 2;
    q = (sum - rootD) / 2;

    return (p*q == n);
}