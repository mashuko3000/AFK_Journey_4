//
// Created by maria on 19.04.2026.
//

#ifndef CRYPT_NTRUPOLYNOMIAL_HPP
#define CRYPT_NTRUPOLYNOMIAL_HPP

#include<vector>
#include <stdexcept>
#include <random>
#include <algorithm>
#include "../math/NTT.hpp"

namespace crypto
{
    using ll = long long;
    class NtruPolynomial final
    {
    private:
        std::vector<ll> coeffs_; //vector of coeffs; [i] is coeff for x^i
        int N_; // thats prime number like x^N - 1
        ll mod_; // p or q : gcd(p, q) == 1

    public:
        explicit NtruPolynomial(int N, ll mod) : N_(N), mod_(mod), coeffs_(N, 0)
        {
            if(N <= 0) throw std::invalid_argument("N must be positive");
            if(mod <= 1) throw std::invalid_argument("mod must be > 1");
        }

        explicit NtruPolynomial(const std::vector<ll> coeffs, int N, ll mod) : N_(N), mod_(mod), coeffs_(std::move(coeffs))
        {
            if(N <= 0) throw std::invalid_argument("N must be positive");
            if(mod <= 1) throw std::invalid_argument("mod must be > 1");

            if (coeffs_.size() > static_cast<size_t>(N_)) coeffs_.resize(N_);
            while(coeffs_.size() < static_cast<size_t>(N_)) coeffs_.push_back(0);
            for(auto& c : coeffs_) c = safeMod(c);
        }

        //GETTERS
        [[nodiscard]] int getN() const {return N_;}
        [[nodiscard]] ll getMod() const {return mod_;}
        [[nodiscard]] const std::vector<ll> getCoeffs() const {return coeffs_;}

        //ACCESS
        ll& operator[](size_t index) {
            if (index >= static_cast<size_t>(N_)) throw std::out_of_range("Polynomial index out of range");
            return coeffs_[index];
        }
        const ll& operator[](size_t index) const {return coeffs_[index];}

    private:
        void checkCompatibility(const NtruPolynomial& other) const
        {
            if (N_ != other.N_ || mod_ != other.mod_) throw std::invalid_argument("Polynomials must have same N and modulus");
        }
    public:
        NtruPolynomial operator+(const NtruPolynomial& other) const
        {
            NtruPolynomial res = *this;
            res += other;
            return res;
        }

        NtruPolynomial operator-(const NtruPolynomial& other) const
        {
            NtruPolynomial res = *this;
            res -= other;
            return res;
        }

        NtruPolynomial operator*(const NtruPolynomial& other) const
        {
            NtruPolynomial res = *this;
            res *= other;
            return res;
        }
        NtruPolynomial operator*(ll scalar) const
        {
            NtruPolynomial res(N_, mod_);
            for(int i = 0; i < N_; ++i)
            {
                res.coeffs_[i] = (coeffs_[i] * scalar) % mod_;
                if (res.coeffs_[i] < 0) res.coeffs_[i] += mod_;
            }
            return res;
        }
        //ARITHMETIC
        NtruPolynomial& operator+=(const NtruPolynomial& other)
        {
            checkCompatibility(other);
            for(int i = 0; i < N_; ++i)
            {
                coeffs_[i] = safeMod(coeffs_[i] + other.coeffs_[i]);
            }
            return *this;
        }

        NtruPolynomial& operator-=(const NtruPolynomial& other)
        {
            checkCompatibility(other);
            for(int i = 0; i < N_; ++i)
            {
                coeffs_[i] = safeMod(coeffs_[i] - other.coeffs_[i]);
            }
            return *this;
        }


        NtruPolynomial& operator*=(const NtruPolynomial& other)
        {
            std::vector<ll> new_coeffs = multiply_cyclic(coeffs_, other.coeffs_);
            coeffs_.assign(N_, 0);

            for(size_t i = 0; i < new_coeffs.size(); ++i)
            {
                coeffs_[i % N_] = (coeffs_[i % N_] + new_coeffs[i]) % mod_;
            }

            for(int i = 0; i < N_; ++i)
            {
                coeffs_[i] = (coeffs_[i] % mod_ + mod_) % mod_;
            }
            return *this;
        }


    public:

        void center()
        {
            ll half = mod_ / 2;
            for(auto& c : coeffs_)
            {
                if(c > half)
                {
                    c -= mod_;
                }
                else if(c <= - half)
                {
                    c+=mod_;
                }
            }
        }

        std::pair<std::vector<long long>, std::vector<long long>> polyDivMod(std::vector<long long> a, std::vector<long long> b, long long mod)
        {
            while (!a.empty() && a.back() == 0) a.pop_back();
            while (!b.empty() && b.back() == 0) b.pop_back();
            if (b.empty()) throw std::runtime_error("Division by zero polynomial");

            int n = a.size();
            int m = b.size();
            std::vector<long long> q(std::max(0, n - m + 1), 0);
            std::vector<long long> r = a;

            long long b_lead_inv = modInverse(b.back(), mod);

            for (int i = n - 1; i >= m - 1; --i)
            {
                long long factor = (r[i] * b_lead_inv) % mod;
                q[i - m + 1] = factor;
                for (int j = 0; j < m; ++j)
                {
                    r[i - m + 1 + j] = (r[i - m + 1 + j] - factor * b[j]) % mod;
                    if (r[i - m + 1 + j] < 0) r[i - m + 1 + j] += mod;
                }
            }
            while (!r.empty() && r.back() == 0) r.pop_back();
            return {q, r};
        }

        long long modInverse(long long a, long long m) const
        {
            long long m0 = m, t, q;
            long long x0 = 0, x1 = 1;
            if (m == 1) return 0;
            while (a > 1)
            {
                q = a / m;
                t = m;
                m = a % m, a = t;
                t = x0;
                x0 = x1 - q * x0;
                x1 = t;
            }
            if (x1 < 0) x1 += m0;
            return x1;
        }

        NtruPolynomial changeMod(ll newMod)
        {
            if(newMod <= 1) throw std::invalid_argument("newMod must be > 1");
            NtruPolynomial res(N_, newMod);
            ll half = mod_ / 2;

            for(int i = 0; i < N_; ++i)
            {
                ll val = coeffs_[i];

                if (val > half)
                {
                    val -= mod_;
                }

                res.coeffs_[i] = res.safeMod(val);
            }
            return res;
        }

        NtruPolynomial invertModPrime(long long mod)
        {
            std::vector<ll> a = coeffs_;
            std::vector<ll> b(N_ + 1, 0);

            b[N_] = 1;
            b[0] = mod - 1;

            std::vector<long long> s1 = {1}, s2 = {0};

            while (!b.empty())
            {
                auto [q, r] = polyDivMod(a, b, mod);

                std::vector<long long> q_s2 = schoolbook_multiply(q, s2, mod);
                std::vector<long long> s_next(std::max(s1.size(), q_s2.size()), 0);

                for (size_t i = 0; i < s_next.size(); ++i)
                {
                    long long v1 = (i < s1.size()) ? s1[i] : 0;
                    long long v2 = (i < q_s2.size()) ? q_s2[i] : 0;

                    s_next[i] = (v1 - v2) % mod;
                    if (s_next[i] < 0) s_next[i] += mod;
                }

                a = b;
                b = r;

                s1 = s2;
                s2 = s_next;
            }

            if (a.size() != 1) throw std::runtime_error("Inverse does not exist (GCD is not a constant)");

            long long inv_gcd = power(a[0], mod - 2, mod);
            for (auto& c : s1) c = (c * inv_gcd) % mod;

            return NtruPolynomial(s1, N_, mod);
        }

        NtruPolynomial invertModuloPowerOfTwo()
        {
            NtruPolynomial fInv = invertModPrime(2);
            long long current_mod = 2;

            while (current_mod < mod_)
            {
                current_mod *= current_mod;
                if (current_mod > mod_) current_mod = mod_;

                NtruPolynomial f_curr = changeMod(current_mod);
                NtruPolynomial inv_curr = fInv.changeMod(current_mod);

                NtruPolynomial prod = f_curr * inv_curr;

                NtruPolynomial diff(N_, current_mod);
                for (int i = 0; i < N_; ++i)
                {
                    long long two = (i == 0) ? 2 : 0;
                    diff[i] = (two - prod[i] + current_mod) % current_mod;
                }

                fInv = inv_curr * diff;
            }
            return fInv;
        }

        static NtruPolynomial generateTernary(int N, ll mod, int num_ones, int num_min_one)
        {
            if(num_min_one + num_ones > N)
            {
                throw std::invalid_argument("Too many non-zero coefficients for this N");
            }

            std::vector<ll> v(N, 0);
            for(int i = 0; i < num_ones; ++i) v[i] = 1;
            for(int i = 0; i < num_min_one; ++i) v[num_ones + i] = -1;

            std::random_device rd;
            std::mt19937 gen(rd());
            std::shuffle(v.begin(), v.end(), gen);

            NtruPolynomial res(N, mod);
            for(int i = 0; i < N; ++i)
            {
                res.coeffs_[i] = (v[i] % mod + mod) % mod;
            }
            return res;
        }
    private:
        ll safeMod(ll coeff)
        {
            ll res = coeff % mod_;
            return res < 0 ? res + mod_ : res;
        }
    };
}

#endif //CRYPT_NTRUPOLYNOMIAL_HPP
