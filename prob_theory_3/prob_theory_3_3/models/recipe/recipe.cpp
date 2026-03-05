//
// Created by mvideo on 01/03/26.
//

#include "recipe.hpp"
#include<stdexcept>
recipe::recipe(int result_id, const std::vector<ingredients>& ingredients_list, int time)
{
    if (result_id < 0)
    {
        throw std::invalid_argument("result item ID cannot be negative");
    }
    if (ingredients_list.empty())
    {
        throw std::invalid_argument("recipe must have at least one ingredient");
    }
    for(const auto& ing : ingredients_list)
    {
        if(ing.count <= 0)
        {
            throw std::invalid_argument("Ingredient count must be greater than zero");
        }
    }
    if (time <= 0)
    {
        throw std::invalid_argument("Production time must be greater than zero");
    }

    this->result_item_id = result_id;
    this->inputs = ingredients_list;
    this->base_production_time = time;
}
int recipe::get_result_id() const
{
    return result_item_id;
}
const std::vector<ingredients>& recipe::get_inputs() const
{
    return inputs;
}
int recipe::get_base_time() const
{
    return base_production_time;
}