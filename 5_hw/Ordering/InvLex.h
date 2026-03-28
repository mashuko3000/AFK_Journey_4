//
// Created by maria on 28.03.2026.
//

#ifndef INC_5_HW_INVLEX_H
#define INC_5_HW_INVLEX_H

#include "interfaces.h"

template <typename T>
class InvLexOrder final : public MonomialOrder<T>
{
public:
    ~InvLexOrder() override = default;
    [[nodiscard]] bool compare(const std::vector<int>& a,
                               const std::vector<int>& b) const override
    {
        MonomialOrder<T>::validateDimension(a, b);

        for (int i = a.size() - 1 ; i >= 0; --i)
        {
            if(a[i] != b[i]) return a[i] > b[i];
        }
        return false;
    }
};

#endif //INC_5_HW_INVLEX_H
