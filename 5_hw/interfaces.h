//
// Created by maria on 28.03.2026.
//

#ifndef INC_5_HW_INTERFACES_H
#define INC_5_HW_INTERFACES_H

#include "Monomial.hpp"

#include<stdexcept>

template<typename T>
class MonomialOrder
{
public:
    virtual ~MonomialOrder() = default;
    [[nodiscard]]virtual bool compare(const std::vector<int>& a,
                         const std::vector<int>& b) const = 0;
protected:
    static void validateDimension(
            const std::vector<int>& a,
            const std::vector<int>& b
            )
    {
        if(a.size() != b.size())
        {
            throw std::invalid_argument("Monomials must have the same number of variables to be compared.");
        }
    }
};

#endif //INC_5_HW_INTERFACES_H
