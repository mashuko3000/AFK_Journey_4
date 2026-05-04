#include "../inc/bigfloat.hpp"

using big_int = boost::multiprecision::cpp_int;

bool bigfloat::operator==(const bigfloat &second) const {
    big_int this_num = numerator_ * second.denominator_;
    big_int other_num = second.numerator_ * denominator_;
    return this_num == other_num;
}

bool bigfloat::operator!=(const bigfloat &second) const {
    return !(*this == second);
}

bool bigfloat::operator<(const bigfloat &second) const {
    if (denominator_ < 0 && second.denominator_ > 0) {
        return true;
    }
    if (denominator_ > 0 && second.denominator_ < 0) {
        return false;
    }
    if (denominator_ < 0 && second.denominator_ < 0) {
        big_int this_num = numerator_ * boost::multiprecision::abs(second.denominator_);
        big_int other_num = second.numerator_ * boost::multiprecision::abs(denominator_);
        return this_num > other_num;
    }
    big_int this_num = numerator_ * second.denominator_;
    big_int other_num = second.numerator_ * denominator_;
    return this_num < other_num;
}

bool bigfloat::operator>(const bigfloat &second) const {
    return !(*this <= second);
}

bool bigfloat::operator<=(const bigfloat &second) const {
    return *this < second || *this == second;
}

bool bigfloat::operator>=(const bigfloat &second) const {
    return *this > second || *this == second;
}
