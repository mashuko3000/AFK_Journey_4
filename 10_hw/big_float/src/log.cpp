#include "../inc/bigfloat.hpp"

bigfloat bigfloat::ln(const bigfloat &eps) const {
    if (*this <= bigfloat(0)) {
        throw std::invalid_argument("Logarithm undefined for non-positive values");
    }

    bigfloat x = *this;
    big_int k(0);

    // Сведение аргумента к диапазону [0.5, 1.0] делением на степени 2
    while (x > bigfloat(1)) {
        x /= bigfloat(2);
        k += 1;
    }
    while (x < bigfloat(0.5)) {
        x *= bigfloat(2);
        k -= 1;
    }

    // Вычисление ln(2) с нужной точностью (только один раз!)
    static bigfloat ln2 = bigfloat(2).ln_internal(eps); // Используем внутреннюю функцию для вычисления ln(2)

    // Вычисление ln(x) для x в диапазоне [0.5, 1.0] с помощью альтернативного ряда
    bigfloat w = (x - bigfloat(1)) / (x + bigfloat(1));
    bigfloat w_squared = w * w;
    bigfloat term = w;
    bigfloat result = w;
    bigfloat n = bigfloat(3);

    while (term.abs() > eps) {
        term *= w_squared;
        result += term / n;
        n += bigfloat(2);
    }

    return result * bigfloat(2) + bigfloat(k) * ln2; // ln(x) = ln(a) + k * ln(2)
}

// Внутренняя функция для вычисления ln(x) без проверки аргументов, нужна для вычисления ln(2)
bigfloat bigfloat::ln_internal(const bigfloat &eps) const {
    bigfloat x = *this;
    bigfloat w = (x - bigfloat(1)) / (x + bigfloat(1));
    bigfloat w_squared = w * w;
    bigfloat term = w;
    bigfloat result = w;
    bigfloat n = bigfloat(3);

    while (term.abs() > eps) {
        term *= w_squared;
        result += term / n;
        n += bigfloat(2);
    }

    return result * bigfloat(2);
}

bigfloat bigfloat::log2(const bigfloat &eps) const {
    if (*this <= bigfloat(0)) {
        throw std::invalid_argument("Logarithm undefined for non-positive values");
    }
    bigfloat ln2 = bigfloat(2).ln(eps);
    return this->ln(eps) / ln2;
}

bigfloat bigfloat::log10(const bigfloat &eps) const {
    if (*this <= bigfloat(0)) {
        throw std::invalid_argument("Logarithm undefined for non-positive values");
    }
    bigfloat ln10 = bigfloat(10).ln(eps);
    return this->ln(eps) / ln10;
}