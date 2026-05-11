#include "Stats.hpp"

static Stats currStats;

template<typename T>
class OpCounter
{
    public:
    T val;

    OpCounter() : val(0) {}
    OpCounter(T v) : val(v) {}
    OpCounter(int v) : val(T(v)) {}

    OpCounter<T>& operator+=(const OpCounter<T>& other)
    {
        currStats.ops++;
        val += other.val;
        return *this;
    }

    OpCounter<T>& operator-=(const OpCounter<T>& other)
    {
        currStats.ops++;
        val -= other.val;
        return *this;
    }

    OpCounter<T>& operator*=(const OpCounter<T>& other)
    {
        currStats.ops++;
        val *= other.val;
        return *this;
    }

    OpCounter<T>& operator/=(const OpCounter<T>& other)
    {
        currStats.ops++;
        val /= other.val;
        return *this;
    }

    OpCounter operator+(const OpCounter<T>& other) const
    {
        OpCounter<T> res(*this);
        return res += other;
    }

    OpCounter<T> operator-(const OpCounter<T>& other) const
    {
        OpCounter<T> res(*this);
        return res -= other;
    }

    OpCounter<T> operator*(const OpCounter<T>& other) const
    {
        OpCounter<T> res(*this);
        return res *= other;
    }

    OpCounter<T> operator/(const OpCounter<T>& other) const
    {
        OpCounter<T> res(*this);
        return res /= other;
    }

    OpCounter<T> fastPow(int exp) const
    {
        OpCounter<T> res = 1;
        OpCounter<T> base = *this;
        while(exp > 0)
        {
            if(exp & 1) res *= base;
            base *= base;
            exp >>= 1;
        }
        return res;
    }

    OpCounter<T> operator-() const
    {
        currStats.ops++;
        return OpCounter<T>(-val);
    }

    OpCounter<T> operator/(int d) const
    {
        currStats.ops++;
        return OpCounter<T>(val / d);
    }

    T raw() const {return val;}
};