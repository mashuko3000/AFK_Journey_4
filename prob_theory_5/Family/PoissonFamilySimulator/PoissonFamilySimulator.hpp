//
// Created by mvideo on 16/03/26.
//

#ifndef FAMILY_POISSONFAMILYSIMULATOR_HPP
#define FAMILY_POISSONFAMILYSIMULATOR_HPP

#include "../interfaces.hpp"
#include<memory>
#include "../math/idistribution.hpp"
#include <random>

class PoissonFamilySimulator : public IFamilySimulator
{
private:
    std::unique_ptr<i_distribution_model> d_children;
    std::unique_ptr<i_distribution_model> d_gender;

public:
    explicit PoissonFamilySimulator(std::unique_ptr<i_distribution_model> children_dist,
                                    std::unique_ptr<i_distribution_model> gender_dist);
    ~PoissonFamilySimulator() override = default;
    FamilyResult simulateFamily() override;
};


#endif //FAMILY_POISSONFAMILYSIMULATOR_HPP
