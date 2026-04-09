//
// Created by maria on 07.04.2026.
//

#ifndef CRYPT_POLYNOMIAL_HPP
#define CRYPT_POLYNOMIAL_HPP

#include "../common/Common.h"

#include <cstdint>
#include<string>
#include <stdexcept>

namespace crypto
{
    template <size_t N>
    class GFPolynomial final
    {
    private:
        uint64_t _value = 0;

    public:
        constexpr GFPolynomial() noexcept = default;
        explicit GFPolynomial(uint64_t val) : _value(val)
        {
            if(getDegree(val) > static_cast<int>(N))
            {
                throw std::invalid_argument("Polynomial degree > N (\" + std::to_string(N) + \")");
            }
        }

        ~GFPolynomial() noexcept = default;

    public:
        constexpr GFPolynomial(const GFPolynomial& other) noexcept = default;
        constexpr GFPolynomial& operator=(const GFPolynomial& other) noexcept = default;

    public:
        constexpr GFPolynomial(GFPolynomial&& other) noexcept = default;
        constexpr GFPolynomial& operator=(GFPolynomial&& other) noexcept = default;

    public:
        // GETTERS
        [[nodiscard]] constexpr uint64_t value() const noexcept
        {
            return _value;
        }

        [[nodiscard]] constexpr int degree() const noexcept
        {
            return getDegree(_value);
        }

    public:
        //OPERATORS

        [[nodiscard]] constexpr GFPolynomial operator+(const GFPolynomial& other) const noexcept
        {
            return GFPolynomial{_value ^ other._value};
        }

        [[nodiscard]] GFPolynomial multiplyMod(
                const GFPolynomial& other,
                const GFPolynomial& modulus
                ) const
        {
            if(modulus.degree() != static_cast<int>(N))
            {
                throw std::invalid_argument("Modulus must have degree exactly N");
            }

            uint64_t p = 0;
            uint64_t a = _value;
            uint64_t b = other._value;
            const uint64_t m = modulus._value;

            while(b != 0)
            {
                if( b& 1) p^= a;
                uint64_t carry = a & (1ULL << (N - 1));
                a <<= 1;
                if(carry) a^= m;
                b >>= 1;
            }
            return GFPolynomial{p};
        }

        [[nodiscard]] GFPolynomial operator%(const GFPolynomial& modulus) const
        {
            if(modulus.value() == 0)
            {
                throw std::domain_error("Modulo by zero");
            }

            uint64_t rem = _value;
            const int m_deg = modulus.degree();

            while(1)
            {
                int r_deg = getDegree(rem);
                if(r_deg < m_deg) break;
                rem ^= (modulus.value() << (r_deg - m_deg));
            }

            return GFPolynomial{rem};
        }

        [[nodiscard]] GFPolynomial inverse(const GFPolynomial& modulus) const
        {
            if(_value == 0) return GFPolynomial{0};

            if(modulus.degree() != static_cast<int>(N))
            {
                throw std::invalid_argument("Modulus degree must be N");
            }

            auto res = xgcd(*this, modulus);
            if(res.gcd.value() != 1)
            {
                throw std::invalid_argument("Inverse does not exist");
            }
            return res.s % modulus;
        }

        [[nodiscard]] bool isIrreducible() const
        {
            int d = degree();
            if(d<= 0) return false;
            if(d == 1) return true;

            const uint64_t limit = 1ULL << (d / 2 + 1);
            for(uint64_t i = 2; i <limit; ++i)
            {
                GFPolynomial divisor{i};
                if(divisor.degree() > d / 2) break;
                if((*this % divisor).value() == 0) return false;
            }
            return true;
        }

        [[nodiscard]] std::string toString() const
        {
            if (_value == 0) return "0";
            std::string s;
            for (int i = degree(); i >= 0; --i)
            {
                if (_value & (1ULL << i))
                {
                    if (!s.empty()) s += " + ";
                    s += (i == 0) ? "1" : (i == 1 ? "x" : "x^" + std::to_string(i));
                }
            }
            return s;
        }

        constexpr  bool operator==(const GFPolynomial& other) const noexcept
        {
            return _value == other._value;
        }

    private:
        [[nodiscard]] static constexpr int getDegree(uint64_t v) noexcept
        {
            if(v == 0) return -1;
            int deg = 0;
            while(v >>= 1) ++deg;
            return deg;
        }

        [[nodiscard]] static ExtendedGCDResult<GFPolynomial<N>> xgcd(GFPolynomial<N> a, GFPolynomial<N> b) noexcept
        {
            GFPolynomial<N> r0 = a, r1 = b;
            GFPolynomial<N> s0{1}, s1{0};
            GFPolynomial<N> t0{0}, t1{1};

            while (r1.value() != 0)
            {
                int shift = r0.degree() - r1.degree();
                if (shift < 0)
                {
                    std::swap(r0, r1); std::swap(s0, s1); std::swap(t0, t1);
                    continue;
                }

                uint64_t r2v = r0.value() ^ (r1.value() << shift);
                uint64_t s2v = s0.value() ^ (s1.value() << shift);
                uint64_t t2v = t0.value() ^ (t1.value() << shift);

                r0 = r1; s0 = s1; t0 = t1;
                r1 = GFPolynomial<N>{r2v};
                s1 = GFPolynomial<N>{s2v};
                t1 = GFPolynomial<N>{t2v};
            }
            return {r0, s0, t0};
        }
    };
}
#endif //CRYPT_POLYNOMIAL_HPP
