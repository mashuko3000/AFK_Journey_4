//
// Created by mvideo on 22/03/26.
//

#ifndef INC_4_HW_ZMOD_HPP
#define INC_4_HW_ZMOD_HPP

#include<iostream>

template <int Mod>
class ZMod
{
private:
    int value;

public:
    ZMod(int v = 0)
    {
        value = v % Mod;
        if (value < 0) value += Mod;
    }

    ZMod operator+(const ZMod& other) const {
        return ZMod(value + other.value);
    }
    ZMod operator-(const ZMod& other) const
    {
        return ZMod(value - other.value);
    }
    ZMod operator*(const ZMod& other) const
    {
        return ZMod(value * other.value);
    }

    ZMod& operator+=(const ZMod& other)
    {
        *this = *this + other; return *this;
    }
    ZMod& operator-=(const ZMod& other)
    {
        *this = *this - other; return *this;
    }
    ZMod& operator*=(const ZMod& other)
    {
        *this = *this * other; return *this;
    }

    bool operator==(const ZMod& other) const
    {
        return value == other.value;
    }
    bool operator!=(const ZMod& other) const
    {
        return value != other.value;
    }

    friend std::ostream& operator<<(std::ostream& os, const ZMod& z)
    {
        return os << z.value;
    }
};

#endif //INC_4_HW_ZMOD_HPP
