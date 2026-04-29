//
// Created by maria on 07.04.2026.
//

#ifndef CRYPT_GALOISFIELD_HPP
#define CRYPT_GALOISFIELD_HPP

#include"Polynomial.hpp"

namespace crypto
{
    template<size_t N>
    class GaloisField final
    {
        GFPolynomial<N> _irreducible;

    public:
        explicit GaloisField(GFPolynomial<N> irr) : _irreducible(std::move(irr))
        {
            if (_irreducible.degree() != static_cast<int>(N))
                throw std::invalid_argument("Irreducible polynomial must have degree exactly N");

            if (!_irreducible.isIrreducible())
                throw std::invalid_argument("Polynomial is not irreducible");
        }

        [[nodiscard]] GFPolynomial<N> add(const GFPolynomial<N>& a, const GFPolynomial<N>& b) const noexcept
        {
            return a + b;
        }

        [[nodiscard]] GFPolynomial<N> mul(const GFPolynomial<N>& a, const GFPolynomial<N>& b) const
        {
            return a.multiplyMod(b, _irreducible);
        }

        [[nodiscard]] GFPolynomial<N> inverse(const GFPolynomial<N>& a) const
        {
            return a.inverse(_irreducible);
        }

        [[nodiscard]] const GFPolynomial<N>& modulus() const noexcept
        {
            return _irreducible;
        }

        [[nodiscard]] bool isIrreducibleModulus() const noexcept
        {
            return true;
        }
    };
}

#endif //CRYPT_GALOISFIELD_HPP
