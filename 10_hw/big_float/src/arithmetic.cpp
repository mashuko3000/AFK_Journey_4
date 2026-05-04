#include "../inc/bigfloat.hpp"


bigfloat &bigfloat::operator+=(const bigfloat &other) & {
    if (denominator_ == 0 || other.denominator_ == 0) {
        throw std::invalid_argument("Zero denominator in +=");
    }
    stat.count_operations++;
    numerator_ = numerator_ * other.denominator_ + other.numerator_ * denominator_;
    denominator_ = denominator_ * other.denominator_;
    simplify();
    return *this;
}

bigfloat &bigfloat::operator-=(const bigfloat &other) & {
    stat.count_operations++;
    numerator_ = numerator_ * other.denominator_ - other.numerator_ * denominator_;
    denominator_ = denominator_ * other.denominator_;
    simplify();
    return *this;
}

bigfloat &bigfloat::operator*=(const bigfloat &other) & {
    stat.count_operations++;
    numerator_ = numerator_ * other.numerator_;
    denominator_ = denominator_ * other.denominator_;
    simplify();
    return *this;
}

bigfloat &bigfloat::operator/=(const bigfloat &other) & {
    if (other.numerator_ == 0) {
        throw std::invalid_argument("Division by zero");
    }
    stat.count_operations++;
    numerator_ *= other.denominator_;
    denominator_ *= other.numerator_;
    simplify();
    return *this;
}

bigfloat bigfloat::operator%(const bigfloat &other) const {
    stat.count_operations++;
    bigfloat quotient = *this/other;
    big_int num = quotient.numerator_;
    big_int den = quotient.denominator_;
    big_int q = num/den;
    bigfloat result = *this - other * bigfloat(q);
    result.simplify();
    return result.abs();
}

bigfloat bigfloat::operator+(const bigfloat &other) const {
    bigfloat result(*this);
    return result += other;
}

bigfloat bigfloat::operator-(const bigfloat &other) const {
    bigfloat result(*this);
    return result -= other;
}

bigfloat bigfloat::operator*(const bigfloat &other) const {
    bigfloat result(*this);
    return result *= other;
}

bigfloat bigfloat::operator/(const bigfloat &other) const {
    bigfloat result(*this);
    return result /= other;
}
