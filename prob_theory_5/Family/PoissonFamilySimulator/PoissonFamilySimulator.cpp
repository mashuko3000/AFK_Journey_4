//
// Created by mvideo on 16/03/26.
//

#include "PoissonFamilySimulator.hpp"
#include <cmath>
#include <stdexcept>

PoissonFamilySimulator::PoissonFamilySimulator(
        std::unique_ptr<i_distribution_model> children_dist,
        std::unique_ptr<i_distribution_model> gender_dist)
        : d_children(std::move(children_dist)),
          d_gender(std::move(gender_dist))
{
    if(!d_children || !d_gender)
    {
        throw std::invalid_argument("Distributions is null");
    }
}
FamilyResult PoissonFamilySimulator::simulateFamily()
{
    int children = static_cast<int>(d_children->get_next_index());
    int girls = 0;

    for(int i = 0; i < children; ++i)
    {
        girls += static_cast<int>(d_gender->get_next_index());
    }

    return {children, girls};
}