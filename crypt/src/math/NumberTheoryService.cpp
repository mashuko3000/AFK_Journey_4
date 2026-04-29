#include "math/NumberTheoryService.hpp"
#include "../inc/core/IPrimalityTest.hpp"
#include <set>
#include <algorithm>

using bigint = boost::multiprecision::cpp_int;

bigint NumberTheoryService::legendreSymb(bigint& num, bigint& p)
{
    if (p < 2) throw std::invalid_argument("p should be primitive and more than 1");
    if (num % p == 0) return 0;

    bigint res = modPow(num, (p - 1) / 2, p);
    if (res == p - 1) return -1;
    return res.convert_to<int>();
}
bigint NumberTheoryService::jacobiSym(bigint& a, bigint& n)
{
    if (n <= 0 || n % 2 == 0)
    {
        throw std::invalid_argument("n should be positive and odd");
    }
    a = a % n;
    if (a < 0) a += n;

    int t = 1;
    while (a != 0)
    {
        while (a % 2 == 0)
        {
            a /= 2;
            bigint r = n % 8;
            if (r == 3 || r == 5)
            {
                t = -t;
            }
        }
        swap(a, n);
        if (a % 4 == 3 && n % 4 == 3)
        {
            t = -t;
        }
        a %= n;
    }
    if (n == 1) return t;
    return 0;
}
bigint NumberTheoryService::gcd(bigint a, bigint b)
{
    a = abs(a);
    b= abs(b);
    while(b!=0)
    {
        bigint t = b;
        b = a % b;
        a = t;
    }
    return a;
}
bigint NumberTheoryService::extended_gcd(bigint a, bigint b, bigint& x, bigint& y)
{
    if(b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    bigint x1, y1;
    bigint d = extended_gcd(b, a%b, x1, y1);
    x = y1;
    y = x1 - y1 * (a/b);
    return d;
}
bigint NumberTheoryService::modPow(bigint base,bigint exp,bigint mod)
{
    if (mod <= 0) throw std::invalid_argument("Module should be positive");
    if (exp < 0) throw std::invalid_argument("Cant do negative exp");

    bigint res = 1;
    base = base % mod;
    if (base < 0) base += mod;

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            res = (res * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}
bigint NumberTheoryService::reverse(bigint a, bigint n)
{
    if(n<= 0) throw std::invalid_argument("Module n should be positive");
    bigint x, y;
    bigint g = extended_gcd(a, n, x, y);
    if(g != 1)
    {
        throw std::invalid_argument("Reverse element doesnt exist");
    }
    return (x % n + n) % n;
}
bigint NumberTheoryService::eulerByFactorization(bigint n)
{
    if (n <= 0) return 0;
    bigint result = n;
    for (bigint p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1)
    {
        result -= result / n;
    }
    return result;
}
bigint NumberTheoryService::eulerByDFT(bigint n)
{
    if (n <= 0) return 0;
    double sum = 0;
    const double pi = std::acos(-1.0);

    double n_double = n.convert_to<double>();

    for (bigint k = 1; k <= n; ++k) {
        double k_double = k.convert_to<double>();
        double gcd_double = gcd(k, n).convert_to<double>();
        sum += gcd_double * std::cos(2.0 * pi * k_double / n_double);
    }
    return bigint(std::round(sum));
}

std::vector<bigint> NumberTheoryService::searchPrimitiveRootInZn(bigint& n, IPrimalityTest& test)
{
    if(n < 2) return {};
    if(n == 2) return {1};
    if(n == 4) return {3};

    bigint p = -1;
    bigint phiN = 0;

    if(!(n & 1))
    {
        bigint m = n >> 1;
        if(!(m & 1)) return {};
        p = getPrimeBase(m, test);
        if(p == -1 || p == 2) return{};
        phiN = (m / p) * (p - 1);
    }
    else
    {
        p = getPrimeBase(n, test);
        if(p == -1 || p == 2) return{};
        phiN = (n / p) * (p - 1);
    }

    std::set<bigint> factors;
    getFactors(phiN, factors, test);

    bigint g = -1;
    for(bigint res = 2; res < n; ++res)
    {
        if(gcd(res, n) != 1)
        {
            continue;
        }

        bool is_root = std::none_of(factors.begin(), factors.end(), [&](const bigint& q) {
            return modPow(res, phiN / q, n) == 1;
        });

        if (is_root) {
            g = res;
            break;
        }
    }

    if(g == -1) return {};

    std::vector<bigint> allRoots;
    bigint current = g;
    for(bigint i = 1; i <= phiN; ++i)
    {
        if(gcd(i, phiN) == 1)
        {
            allRoots.push_back(current);
        }
        current = (current * g) % n;
    }

    return allRoots;
}

bigint NumberTheoryService::rhoPollard(bigint n, IPrimalityTest& test)
{
    if(!(n & 1)) return 2;
    if(test.isPrime(n, 0.9999)) return n;

    bigint x = 2, y = 2, d = 1, c = 1;
    auto f = [&](bigint x, bigint& c, bigint& m)
    {
        return (x * x + c) % m;
    };

    while(d == 1)
    {
        x = f(x, c, n);
        y = f((f(y, c, n)), c, n);
        d = gcd(abs(x - y), n);
        if(d == n)
        {
            x = 2;
            y = 2;
            c++;
            d = 1;
        }
    }
    return d;
}

void NumberTheoryService::getFactors(const bigint& n, std::set<bigint>& factors, IPrimalityTest& test)
{
    if(n == 1) return;
    if(test.isPrime(n, 0.9999))
    {
        factors.insert(n);
        return;
    }
    bigint d = rhoPollard(n, test);
    getFactors(d, factors, test);
    getFactors(n / d, factors, test);
}

bigint NumberTheoryService::getPrimeBase(bigint& n, IPrimalityTest& test)
{
    if(test.isPrime(n, 0.9999)) return n;

    int maxK = (int)boost::multiprecision::msb(n) + 1;
    for(int i = maxK; i >= 2; --i)
    {
        bigint low = 2;
        bigint high = n;
        bigint p = -1;
        bigint mid = 0;

        while(low <= high)
        {
            mid = (low + high) / 2;
            bigint val = boost::multiprecision::pow(mid, i);
            if(val == n)
            {
                p = mid;
                break;
            }
            if(val < n)
            {
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }
        }
        if(p != -1 && test.isPrime(p, 0.9999)) return std::move(p);
    }
    return -1;
}
