//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_3_RECIPE_HPP
#define PROB_THEORY_3_3_RECIPE_HPP

#include"types.hpp"
#include<vector>

class recipe
{
private:
    int result_item_id;
    std::vector<ingredients> inputs;
    int base_production_time;

public:
    recipe(int result_id, const std::vector<ingredients>& ingredients_list, int time);
    int get_result_id() const;
    const std::vector<ingredients>& get_inputs() const;
    int get_base_time() const;
};


#endif //PROB_THEORY_3_3_RECIPE_HPP
