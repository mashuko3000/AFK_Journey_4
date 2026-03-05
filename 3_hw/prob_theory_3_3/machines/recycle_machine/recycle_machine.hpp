//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_3_RECYCLE_MACHINE_HPP
#define PROB_THEORY_3_3_RECYCLE_MACHINE_HPP

#include"base_machine/base_machine.hpp"
#include"recipe/recipe.hpp"
#include"inventory/inventory.hpp"
#include"quality_logic/quality_logic.hpp"
#include "stats_logic/stats_logic.hpp"

class recycle_machine : public base_machine
{
private:
    const recipe& item_recipe;
    quality_level_t input_quality;

public:
    recycle_machine(const recipe& r, quality_level_t q);
    void try_start_recycling(inventory& inv);
    bool tick(inventory& inv);
};


#endif //PROB_THEORY_3_3_RECYCLE_MACHINE_HPP
