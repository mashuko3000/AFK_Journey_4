//
// Created by maria on 28.03.2026.
//

#ifndef INC_5_HW_GRLEX_H
#define INC_5_HW_GRLEX_H

#include "interfaces.h"

template <typename T>
class GrLexOrder final : public MonomialOrder<T>
{
public:
    ~GrLexOrder() override = default;
    [[nodiscard]] bool compare(const std::vector<int>& a,
                               const std::vector<int>& b) const override
    {
        MonomialOrder<T>::validateDimension(a, b);

        int sumA = 0, sumB = 0;

        for(const auto& a_ : a) sumA += a_;
        for(const auto& b_ : b) sumB += b_;

        if(sumA != sumB)
        {
            return sumA>sumB;
        }

        for (size_t i = 0 ; i < a.size(); ++i)
        {
            if(a[i] != b[i]) return a[i] > b[i];
        }
        return false;
    }
};

#endif //INC_5_HW_GRLEX_H
