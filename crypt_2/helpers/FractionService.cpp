//
// Created by maria on 31.03.2026.
//

#include "FractionService.h"

std::vector<bigint> FractionService::getContinuedFraction(const Fraction& fraction) const
{
    validateFraction(fraction);

    std::vector<bigint> cf;
    cf.reserve(16);

    bigint a = fraction.numerator;
    bigint b = fraction.denominator;

    while(b > 0)
    {
        cf.push_back(a/b);
        bigint rem = a%b;
        a = b;
        b = rem;
    }

    return cf;
}

Fraction FractionService::getFractionFromContinued(const std::vector<bigint>& cf) const
{
    if(cf.empty())
    {
        throw std::invalid_argument("Continued fraction sequence cannot be empty.");
    }

    auto convergents = generateConvergentsFromCF(cf);
    return convergents.back();
}

std::vector<Fraction> FractionService::getConvergents(const Fraction& fraction)  const
{
    const std::vector<bigint> cf = getContinuedFraction(fraction);
    return generateConvergentsFromCF(cf);
}

Path FractionService::getCalkinWilfPath(const Fraction& fraction) const
{
    validateFraction(fraction);
    if(fraction.numerator == 0)
    {
        throw std::invalid_argument("Stern-Brocot tree operates on strictly positive rationals.");
    }

    Path reversed_path;
    reversed_path.reserve(32);
    bigint a = fraction.numerator;
    bigint b = fraction.denominator;
    while(a != 1 || b != 1)
    {
        if(a > b)
        {
            reversed_path.push_back(Direction::RIGHT);
            a = a-b;
        }
        else if(b < a)
        {
            reversed_path.push_back(Direction::LEFT);
            b = b - a;
        }
        else
        {
            break;
        }

        std::reverse(reversed_path.begin(), reversed_path.end());
        return reversed_path;
    }
}

Path FractionService::getSternBrocotPath(const Fraction& fraction) const
{
    validateFraction(fraction);
    if(fraction.numerator == 0)
    {
        throw std::invalid_argument("Stern-Brocot tree operates on strictly positive rationals.");
    }

    Path path;
    path.reserve(32);

    Fraction L{0, 1};
    Fraction R{1, 0};
    Fraction M{1, 1};

    while(!(M == fraction))
    {
        if(fraction < M)
        {
            path.push_back(Direction::LEFT);
            R = M;
        }
        if(fraction > M)
        {
            path.push_back(Direction::RIGHT);
            L = M;
        }

        M = {L.numerator + R.numerator, L.denominator + R.denominator};
    }

    return path;
}

Fraction FractionService::getFractionFromCalkinWilfPath(const Path& path) const
{
    bigint a = 1, b = 1;
    for(const Direction& dir : path)
    {
        if(dir == Direction::LEFT)
        {
            b = a + b;
        }
        else if (dir == Direction::RIGHT)
        {
            a = a + b;
        }
    }
    return {a, b};
}

Fraction FractionService::getFractionFromSternBrocotPath(const Path& path) const
{
    Fraction L{0, 1};
    Fraction R{1, 0};
    Fraction M{1, 1};

    for(const Direction& dir : path)
    {
        if(dir == Direction::LEFT)
        {
            R = M;
        }
        else if (dir == Direction::RIGHT)
        {
            L = M;
        }
        M = {L.numerator + R.numerator, L.denominator + R.denominator};
    }
    return M;
}

std::vector<Fraction> FractionService::getConvergentsFromSternBrocotPath(const Path& path) const
{
    if(path.empty())
    {
        return {Fraction{1, 1}};
    }

    std::vector<bigint> cf;
    cf.reserve(16);

    bigint count = 0;
    Direction current_dir = Direction::RIGHT;

    if(path.front() == Direction::LEFT)
    {
        cf.push_back(0);
        current_dir = Direction::LEFT;
    }

    for(const auto& dir : path)
    {
        if(dir == current_dir)
        {
            count ++;
        }
        else
        {
            cf.push_back(count);
            count = 1;
            current_dir = dir;
        }
    }

    cf.push_back(count + 1);

    return generateConvergentsFromCF(cf);
}

void FractionService::validateFraction(const Fraction& f) const
{
    if (f.denominator <= 0)
    {
        throw std::invalid_argument("Fraction denominator must be strictly positive (in N).");
    }
    if (f.numerator < 0)
    {
        throw std::invalid_argument("Fraction numerator must be non-negative (in Z+).");
    }
    if (NumberTheoryService::gcd(f.numerator, f.denominator) != 1)
    {
        throw std::invalid_argument("Fraction must be irreducible (gcd(a, b) == 1).");
    }
}
std::vector<Fraction> FractionService::generateConvergentsFromCF(const std::vector<bigint>& cf) const
{
    if(cf.empty()) return {};

    std::vector<Fraction> convergents;
    convergents.reserve(cf.size());

    bigint p_prev2 = 0, p_prev1 = 1;
    bigint q_prev2 = 1, q_prev1 = 0;

    for(const auto& q_i : cf)
    {
        bigint p = q_i * p_prev1 + p_prev2;
        bigint q = q_i * q_prev1 + q_prev2;

        convergents.push_back({p, q});

        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return convergents;
}