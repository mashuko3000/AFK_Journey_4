//
// Created by mvideo on 17/03/26.
//

#ifndef CIPHERATTACK_STATISTICALATTACK_HPP
#define CIPHERATTACK_STATISTICALATTACK_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <iomanip>

class StatisticalAnalyzer
{
public:

    std::map<char, int> getFrequencies(const std::string& text) const
    {
        std::map<char, int> freqs;
        for (char c : text)
        {
            if (isalpha(c)) freqs[toupper(c)]++;
        }
        return freqs;
    }

    double calculateIC(const std::string& text) const
    {
        auto freqs = getFrequencies(text);
        int n = 0;
        double sum = 0.0;
        for(const auto& [c, count] : freqs)
        {
            sum+=count * (count - 1);
            n+= count;
        }
        if(n<= 1) return 0.0;
        return sum/(n*(n-1));
    }

    std::string extractColumn(const std::string& text, int keyLength, int offset) const
    {
        std::string col = "";
        size_t textSize = text.length();

        for(size_t i = offset; i < textSize; i+=keyLength)
        {
            col+=text[i];
        }
        return col;
    }
};

#endif //CIPHERATTACK_STATISTICALATTACK_HPP
