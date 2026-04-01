//
// Created by maria on 30.03.2026.
//

#include "fermat_attack.h"

[[nodiscard]] std::pair<bigint, bigint> FermatAttack::execute(const bigint& n, const bigint& e) const
{
    if(n <= 0) throw std::invalid_argument("Modulus n must be positive.");
    if(!(n & 1)) return {2, n >> 1};

    bigint a = boost::multiprecision::sqrt(n);

    if(a * a < n)
    {
        a += 1;
    }

    bigint bSq = a * a - n;
    bigint b;

    while(!perfectSquare(bSq, b))
    {
        a += 1;
        bSq = a * a - n;
    }

    bigint p = a - b;
    bigint q = a + b;

    return {std::min(p, q), std::max(p, q)};

}

bool FermatAttack::perfectSquare(const bigint& x, bigint& out_root) const noexcept
{
    if(x < 0) return false;
    out_root = boost::multiprecision::sqrt(x);
    return out_root * out_root == x;
}