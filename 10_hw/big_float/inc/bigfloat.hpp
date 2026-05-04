#ifndef UNTITLED_bigfloat_HPP
#define UNTITLED_bigfloat_HPP
#include<cmath>

#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <string>
#include <boost/multiprecision/cpp_dec_float.hpp>
using big_int = boost::multiprecision::cpp_int;

struct stats
{
    long long count_operations = 0;
    void reset() {count_operations = 0;}
};

class bigfloat{
private:
    static stats stat;

    big_int numerator_;
    big_int denominator_;

private:
    void simplify();
    void parse_number(const char *str, int &index, big_int &result);
    bigfloat reciprocal() const;
    bigfloat ln_internal(const bigfloat &eps) const;

public:
    static big_int power_base(const big_int& base, const big_int& exp);
    static void reset_stats() {stat.reset();}
    static long long get_operations() {return stat.count_operations;}

public:
    static const bigfloat DEFAULT_EPSILON;
    
    bigfloat();
    bigfloat(const char *str);
    bigfloat(const big_int &numerator, const big_int &denominator);
    bigfloat(const big_int &other);
    bigfloat(int other);

    bigfloat inverse() const {
        /*if (denominator_ != 1 || boost::multiprecision::abs(numerator_) != 1) {
            throw std::runtime_error("Coefficient is not invertible in Z! Not exists");
        }*/
        return bigfloat(denominator_, numerator_);
    }

public:
    bigfloat(const bigfloat &other);
    bigfloat(bigfloat &&other) noexcept;
    bigfloat &operator=(const bigfloat &other);
    bigfloat &operator=(bigfloat &&other) noexcept;
    ~bigfloat() = default;

public:
    bigfloat abs() const;

public:
    bigfloat operator-() const;

public:
    bigfloat &operator+=(const bigfloat &other) &;
    bigfloat &operator-=(const bigfloat &other) &;
    bigfloat &operator*=(const bigfloat &other) &;
    bigfloat &operator/=(const bigfloat &other) &;

public:
    bigfloat operator+(const bigfloat &other) const;
    bigfloat operator-(const bigfloat &other) const;
    bigfloat operator*(const bigfloat &other) const;
    bigfloat operator/(const bigfloat &other) const;
    bigfloat operator%(const bigfloat &other) const;

public:
    bool operator==(const bigfloat &second) const;
    bool operator!=(const bigfloat &second) const;
    bool operator<=(const bigfloat &second) const;
    bool operator>=(const bigfloat &second) const;
    bool operator<(const bigfloat &second) const;
    bool operator>(const bigfloat &second) const;

public:
    void parse_number(const char *str, int &index, big_int &result, big_int &denominator);
    std::string to_decimal(int precision) const;
    //char* to_decimal(int precision) const;
    friend std::ostream &operator<<(std::ostream &out, const bigfloat &num) noexcept;
    friend std::istream &operator>>(std::istream &in, bigfloat &num);

public:
    static bigfloat PI(const bigfloat &eps);
    static bigfloat E(const bigfloat& eps, const bigfloat& x = 1);
    bigfloat truncate() const;

public:
    bigfloat pow(unsigned int exp) const;
    bigfloat pow(big_int exp) const;
    bigfloat exp(const bigfloat &eps) const;
    bigfloat sqrt(const bigfloat &eps) const;
    static big_int factorial(const big_int &n);
    static big_int factorial(const unsigned long &n);

public:
    friend bigfloat sin(const bigfloat &base, const bigfloat &eps);
    friend bigfloat cos(const bigfloat &base, const bigfloat &eps);
    friend bigfloat tan(const bigfloat &base, const bigfloat &eps);
    friend bigfloat cot(const bigfloat &base, const bigfloat &eps);
    friend bigfloat sec(const bigfloat &base, const bigfloat &eps);
    friend bigfloat csc(const bigfloat &base, const bigfloat &eps);
    friend bigfloat arcsin(const bigfloat &base, const bigfloat &eps);
    friend bigfloat arccos(const bigfloat &base, const bigfloat &eps);
    friend bigfloat arctg(const bigfloat &base, const bigfloat &eps);
    friend bigfloat arcctg(const bigfloat &base, const bigfloat &eps);
    friend bigfloat arcsec(const bigfloat &base, const bigfloat &eps);
    friend bigfloat arccosec(const bigfloat &base, const bigfloat &eps);

public:
    friend bigfloat binomialCoefficient(unsigned long n, unsigned long k);
    friend bigfloat bernoulli(unsigned long n);

public:
    bigfloat log2(const bigfloat &eps) const;
    bigfloat ln(const bigfloat &eps) const;
    bigfloat log10(const bigfloat &eps) const;
};

big_int factorial(const big_int& n);
big_int power_base(const big_int &base, unsigned int exp);

#endif //UNTITLED_bigfloat_HPP
