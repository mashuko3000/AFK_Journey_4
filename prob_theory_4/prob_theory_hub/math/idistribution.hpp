//
// Created by mvideo on 07/03/26.
//

#ifndef PROB_THEORY_HUB_IDISTRIBUTION_HPP
#define PROB_THEORY_HUB_IDISTRIBUTION_HPP

#include<cstdlib>

class i_distribution_model
{
public:
    virtual ~i_distribution_model() = default;
    virtual size_t get_next_index() = 0;
    virtual i_distribution_model* clone() const = 0;
};

#endif //PROB_THEORY_HUB_IDISTRIBUTION_HPP
