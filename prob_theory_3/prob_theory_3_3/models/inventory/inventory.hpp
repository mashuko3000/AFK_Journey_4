//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_3_INVENTORY_HPP
#define PROB_THEORY_3_3_INVENTORY_HPP

#include <vector>
#include <map>
#include "types.hpp"
#include "constants.hpp"

class inventory {
    //map[ComponentID][QualityLevel] -> Count
    std::map<int, std::vector<int>> storage;
public:
    void clear();
    void add_resources(int item_id, quality_level_t quality, int count);
    void remove_resources(int item_id, quality_level_t quality, int count);
    bool has_enough(int item_id, quality_level_t quality, int count) const;

    bool can_afford_recipe(const std::vector<ingredients>& recipe, quality_level_t quality) const;
    void consume_recipe(const std::vector<ingredients>& recipe, quality_level_t quality);

    void process_recycle_return(const std::vector<ingredients>& original_recipe, quality_level_t quality);

    int get_count(int item_id, quality_level_t quality) const;
};


#endif //PROB_THEORY_3_3_INVENTORY_HPP
