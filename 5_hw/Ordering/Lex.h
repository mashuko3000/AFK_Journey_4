//
// Created by maria on 28.03.2026.
//

#ifndef INC_5_HW_LEX_H
#define INC_5_HW_LEX_H

#include "interfaces.h"

template <typename T>
class LexOrder final : public MonomialOrder<T>
{
public:
    ~LexOrder() override = default;
    [[nodiscard]] bool compare(const std::vector<int>& a,
                               const std::vector<int>& b) const override
    {
        MonomialOrder<T>::validateDimension(a, b);
        for (size_t i = 0 ; i < a.size(); ++i)
        {
            if(a[i] != b[i]) return a[i] > b[i];
        }
        return false;
    }
};
#endif //INC_5_HW_LEX_H
