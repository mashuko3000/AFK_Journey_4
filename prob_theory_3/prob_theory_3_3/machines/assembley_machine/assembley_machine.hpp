//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_3_ASSEMBLEY_MACHINE_HPP
#define PROB_THEORY_3_3_ASSEMBLEY_MACHINE_HPP

#include"base_machine/base_machine.hpp"
#include"recipe/recipe.hpp"
#include"inventory/inventory.hpp"
#include"quality_logic/quality_logic.hpp"
#include "stats_logic/stats_logic.hpp"

class assembly_machine : public base_machine
{
private:
    const recipe& current_recipe;
    quality_level_t target_quality;

public:
    assembly_machine(const recipe& r, quality_level_t q);

    void try_start_production(inventory& inv);

    bool tick(inventory& inv,
              random_generator& rng,
              stats_logic& stats,
              int current_tick);
};


#endif //PROB_THEORY_3_3_ASSEMBLEY_MACHINE_HPP
