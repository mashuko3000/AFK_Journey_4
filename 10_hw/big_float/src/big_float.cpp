#include "../inc/bigfloat.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <cctype>
#include <stdexcept>

using big_int = boost::multiprecision::cpp_int;
const bigfloat bigfloat::DEFAULT_EPSILON = bigfloat("1/10000");
stats bigfloat::stat;

bigfloat::bigfloat() : numerator_(0), denominator_(1) {}

bigfloat::bigfloat(const char *str) {
    if (!str || str[0] == '\0') {
        throw std::invalid_argument("Empty input string");
    }

    int index = 0;
    while (str[index] == ' ') {
        ++index;
    }

    int slash_index = -1;
    int len = index;
    while (str[len] != '\0') {
        if (str[len] == '/') {
            slash_index = len;
            break;
        }
        ++len;
    }

    try {
        if (slash_index == -1) {
            parse_number(str, index, numerator_);
            denominator_ = 1;
        } else {
            parse_number(str, index, numerator_);
            while (str[index] == ' ') {
                ++index;
            }
            if (str[index] != '/') {
                throw std::invalid_argument("Expected '/' after numerator");
            }
            ++index;
            while (str[index] == ' ') {
                ++index;
            }
            parse_number(str, index, denominator_);
            if (denominator_ == 0) {
                throw std::invalid_argument("Denominator cannot be zero");
            }
        }

        while (str[index] == ' ') {
            ++index;
        }
        if (str[index] != '\0') {
            throw std::invalid_argument("Invalid characters after number");
        }

        simplify();
    } catch (...) {
        throw;
    }
}

bigfloat::bigfloat(const big_int &numerator, const big_int &denominator)
        : numerator_(numerator), denominator_(denominator) {
    simplify();
}

bigfloat::bigfloat(const big_int &other) : numerator_(other), denominator_(1) {}

bigfloat::bigfloat(int other) : numerator_(other), denominator_(1) {
    simplify();
}

bigfloat::bigfloat(const bigfloat &other)
        : numerator_(other.numerator_), denominator_(other.denominator_) {}

bigfloat::bigfloat(bigfloat &&other) noexcept
        : numerator_(std::move(other.numerator_)), denominator_(std::move(other.denominator_)) {}

bigfloat &bigfloat::operator=(const bigfloat &other) {
    if (this != &other) {
        numerator_ = other.numerator_;
        denominator_ = other.denominator_;
    }
    return *this;
}

bigfloat &bigfloat::operator=(bigfloat &&other) noexcept {
    if (this != &other) {
        numerator_ = std::move(other.numerator_);
        denominator_ = std::move(other.denominator_);
    }
    return *this;
}


