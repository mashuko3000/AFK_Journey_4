#include "NumberTheoryService.hpp"



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
        b = b % a;
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
