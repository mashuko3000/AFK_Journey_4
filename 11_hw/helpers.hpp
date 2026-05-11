#include "OpCounter.hpp"
#include <vector>

std::vector<std::vector<OpCounter<bigfloat>>> Ccache;

OpCounter<bigfloat> getC(int n, int k)
{
    if(k < 0 || k > n) return OpCounter<bigfloat>(0);

    while (Ccache.size() <= n)
    {
        int row = Ccache.size();
        std::vector<OpCounter<cpp_rational>> current_row(row + 1, OpCounter<cpp_rational>(1));
        
        for (int i = 1; i < row; ++i)
        {
            current_row[i] = Ccache[row - 1][i - 1] + Ccache[row - 1][i];
        }
        Ccache.push_back(current_row);
    }
    return Ccache[n][k];
}

std::vector<OpCounter<cpp_rational>> Bcache;

OpCounter<cpp_rational> getB(int i)
{
    if (Bcache.empty())
    {
        Bcache.push_back(OpCounter<cpp_rational>(1)); 
    }
    
    while (Bcache.size() <= i)
    {
        int j = Bcache.size();
        OpCounter<cpp_rational> sum(0);

        for (int k = 1; k <= j; ++k)
        {
            sum += getC(j + 1, k + 1) * Bcache[j - k];
        }

        OpCounter<cpp_rational> Bj = -sum / (j + 1);
        Bcache.push_back(Bj);
    }
    return Bcache[i];
}

template<typename T>
T calculateExact(int n, int p)
{
    currStats.reset(); 
    
    OpCounter<cpp_rational> N(n);
    OpCounter<cpp_rational> total_sum(0);

    for (int j = 0; j <= p; ++j)
    {
        OpCounter<cpp_rational> term = getC(p + 1, j) * getB(j) * N.fastPow(p + 1 - j);
        total_sum += term;
    }

    OpCounter<cpp_rational> result = total_sum / (p + 1) + N.fastPow(p);
    return static_cast<T>(numerator(result.raw()) / denominator(result.raw()));
}