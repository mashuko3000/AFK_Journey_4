#include"../inc/convert.hpp"

std::string to_polynomial(uint64_t elem, int n) {
    if (n < 2 || n > 64) throw std::invalid_argument("n must be 2-64");
    if (elem == 0) return "0";

    std::string res;
    bool first = true;

    for (int i = 63; i >= 0; --i) {
        if ((elem >> i) & 1) {
            if (!first) res += " + ";
            if (i == 0) res += "1";
            else if (i == 1) res += "x";
            else res += "x^" + std::to_string(i);
            first = false;
        }
    }
    return res;
}

uint64_t from_polynomial(const std::string& str, int n) {
    if (n < 2 || n > 65) throw std::invalid_argument("n must be 2-65");
    if (str == "0") return 0;
    uint64_t res = 0;
    std::string s = str;
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());

    std::stringstream ss(s);
    std::string term;

    while (std::getline(ss, term, '+')) {
        int power = 0;
        if (term == "1") power = 0;
        else if (term == "x") power = 1;
        else if (term.find("x^") == 0) {
            power = std::stoi(term.substr(2));
        } else {
            throw std::invalid_argument("Invalid polynomial format");
        }

        if (power >= n) throw std::out_of_range("Power exceeds field degree n");

        res |= (1ULL << power);
    }
    return res;
}

// ========Task 2==========

uint64_t polynomial_multiply_no_reduce(uint64_t a, uint64_t b)
{
    int deg_a = polynomial_degree(a);
    int deg_b = polynomial_degree(b);

    if (deg_a > 32 || deg_b > 32) {
        throw std::invalid_argument("Степень выше 32 запрещена");
    }

    if (deg_a == 32 && deg_b == 32) {
        throw std::invalid_argument("Оба множителя имеют степень 32 (результат выйдет за 64 бита)");
    }

    uint64_t result = 0;
    while (b)
    {
        if (b & 1) result ^= a;
        a <<= 1;
        b >>= 1;
    }
    return result;
}

// ======utilits==========
int polynomial_degree(uint64_t p) {
    if (p == 0) return -1;
    int deg = 63;
    while (deg >= 0 && (p & (1ULL << deg)) == 0) --deg;
    return deg;
}
uint64_t polynomial_reduce(uint64_t poly, uint64_t modulus, int n) {
    if (poly == 0) return 0;

    while (true) {
        int deg = polynomial_degree(poly);
        if (deg < n) break;

        int shift = deg - n;
        poly ^= (modulus << shift);
    }

    return poly;
}

// ==========Task 3==========

uint64_t multiply(uint64_t a, uint64_t b, uint64_t irr, int n)
{
    if (n < 2 || n > 64)
    {
        throw std::invalid_argument("n must be in [2,64]");
    }

    if (a == 0 || b == 0) return 0;

    if (polynomial_degree(irr) != n)
    {
        throw std::invalid_argument("irreducible must have exact degree n");
    }

    uint64_t p = 0;
    uint64_t high_bit = 1ULL << (n - 1);

    while (b != 0) {
        if (b & 1) {
            p ^= a;
        }

        bool carry = (a & high_bit) != 0;
        a <<= 1;
        if (carry) {
            a ^= irr;
        }

        b >>= 1;
    }

    while (true) {
        int deg = polynomial_degree(p);
        if (deg < n) break;
        int shift = deg - n;
        p ^= (irr << shift);
    }

    return p;
}

//================Task 4===============

gcd extended_gcd(uint64_t a, uint64_t b, uint64_t mod_poly, int n)
{
    uint64_t s0 = 1, s1 = 0;
    uint64_t t0 = 0, t1 = 1;

    uint64_t r0 = a, r1 = b;

    while (r1 != 0) {
        int deg_r0 = polynomial_degree(r0);
        int deg_r1 = polynomial_degree(r1);

        if (deg_r0 < deg_r1) {
            std::swap(r0, r1);
            std::swap(s0, s1);
            std::swap(t0, t1);
            continue;
        }

        int shift = deg_r0 - deg_r1;
        r0 ^= (r1 << shift);

        s0 ^= (s1 << shift);
        t0 ^= (t1 << shift);

        s0 = polynomial_reduce(s0, mod_poly, n);
        t0 = polynomial_reduce(t0, mod_poly, n);
    }

    gcd res;
    res.gcd = r0;
    res.s = s0;
    res.t = t0;
    return res;
}

uint64_t gf2n_inverse(uint64_t a, uint64_t irr, int n) {
    if (a == 0) {
        return 0;
    }

    gcd res = extended_gcd(a, irr, irr, n);

    return res.s;
}