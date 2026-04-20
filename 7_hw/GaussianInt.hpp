#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <stdexcept>
#include <cmath>

class Complex final {
private:
    double real;
    double imag;
    static constexpr double EPS = 1e-9;
public:
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}
    Complex(int r) : real(static_cast<double>(r)), imag(0.0) {}
    Complex(const Complex& other) : real(other.real), imag(other.imag) {}

    Complex& operator=(const Complex& other)
    {
        if (this != &other){
            real = other.real;
            imag = other.imag;
        }
        return *this;
    }

    ~Complex() = default;

    double get_real() const {return real;}
    double get_imag() const {return imag;}

    void set_real(double r) {real = r;}
    void set_imag(double i) {imag = i;}

    Complex& operator+=(const Complex& other) {
        real += other.real;
        imag += other.imag;
        return *this;
    }
    bool operator==(const Complex& o) const {
        return std::abs(real - o.real) < EPS && std::abs(imag - o.imag) < EPS;
    }
    bool operator!=(const Complex& o) const { return !(*this == o); }
    Complex operator+(const Complex& other) const {
        Complex result = *this;
        result += other;
        return result;
    }

    Complex& operator-=(const Complex& other){
        real -= other.real;
        imag -= other.imag;
        return *this;
    }

    Complex operator-(const Complex& other) const{
        Complex result = *this;
        result-=other;
        return result;
    }

    Complex& operator*=(const Complex& other) {
        double old_real = real;
        real = old_real * other.real - imag * other.imag;
        imag = old_real * other.imag + imag * other.real;
        return *this;
    }

    Complex operator*(const Complex& other) const {
        Complex res = *this; res *= other; return res;
    }

    Complex& operator/=(const Complex& other){
        double denom = other.real * other.real + other.imag * other.imag;
        if (denom == 0){
            throw std::invalid_argument("Division by zero ?_?");
        }
        double new_real = (real * other.real + imag * other.imag)/denom;
        double new_imag = (imag * other.real - real * other.imag)/denom;

        real = new_real;
        imag = new_imag;

        return *this;
    }

    Complex operator/(const Complex& other) const
    {
        return *this * other.inverse();
    }

    double absolute() const{
        return std::sqrt(real * real + imag * imag);
    }

    double argument() const{
        return std::atan2(imag, real);
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& c)
    {
        os << "(" << c.real << " + " << c.imag << "i)";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Complex& c){
        is >> c.real >> c.imag;
        return is;
    }

    bool is_zero() const
    {
        return std::abs(real) < 1e-9 && std::abs(imag) < 1e-9;
    }

    Complex inverse() const
    {
        double denom = real * real + imag * imag;
        if (denom < 1e-12) throw std::runtime_error("Division by zero in Complex");
        return Complex(real / denom, -imag / denom);
    }
};

#endif