//
// Created by mvideo on 28/02/26.
//

#ifndef PROB_THEORY_3_1_DISTRIBUTION_MODEL_HPP
#define PROB_THEORY_3_1_DISTRIBUTION_MODEL_HPP

#include<cstdlib>
#include<vector>

class i_distribution_model
{
public:
    virtual ~i_distribution_model() = default;
    virtual size_t get_next_index() = 0;
    virtual i_distribution_model* clone() const = 0;
};

class discrete_distribution_model : public i_distribution_model
{
private:
    std::vector<double> cumulative_prob;
public:
    // weights is  for each element of alphabet
    explicit discrete_distribution_model(const std::vector<double>& weights)
    {
        double sum = 0;
        for(double w : weights) sum += w;

        double cur_sum = 0;
        for(double w : weights)
        {
            cur_sum += w/sum;
            cumulative_prob.push_back(cur_sum);
        }
    }

    size_t get_next_index() override
    {
        double r = static_cast<double>(std::rand()) / RAND_MAX;
        size_t size = cumulative_prob.size();
        for(size_t i = 0; i < size; ++i)
        {
            if (r <= cumulative_prob[i])
            {
                return i;
            }
        }
        return size - 1;
    }

    i_distribution_model* clone() const override { return new discrete_distribution_model(*this); }
};

#endif //PROB_THEORY_3_1_DISTRIBUTION_MODEL_HPP
