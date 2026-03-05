//
// Created by mvideo on 01/03/26.
//

#include "assembley_machine.hpp"

assembly_machine::assembly_machine(const recipe& r, quality_level_t q): current_recipe(r), target_quality(q) {}

void assembly_machine::try_start_production(inventory& inv)
{
    if(is_working) return;
    if(inv.can_afford_recipe(current_recipe.get_inputs(), target_quality))
    {
        inv.consume_recipe(current_recipe.get_inputs(), target_quality);
        double slowdown = 1.0 + (modules_count * constants_t::MODULE_SLOWDOWN_COEFFICIENT);
        ticks_left = static_cast<int>(std::round(current_recipe.get_base_time() * slowdown));

        is_working = true;
    }
}

bool assembly_machine::tick(inventory& inv,
          random_generator& rng,
          stats_logic& stats,
          int current_tick)
{
    if(base_machine::tick())
    {
        quality_level_t result_quality = quality_logic::calculate_result_quality(
                target_quality,
                modules_count,
                rng
        );

        inv.add_resources(current_recipe.get_result_id(), result_quality, 1);
        stats.log_production(result_quality, current_tick);
        return true;
    }
    return false;
}
