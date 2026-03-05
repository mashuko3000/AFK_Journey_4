//
// Created by mvideo on 01/03/26.
//

#include "recycle_machine.hpp"

#include <cmath>


recycle_machine::recycle_machine(const recipe& r, quality_level_t q)
        : item_recipe(r), input_quality(q) {}

void recycle_machine::try_start_recycling(inventory& inv)
{
    if (is_working) return;
    if (inv.has_enough(item_recipe.get_result_id(), input_quality, 1))
    {
        inv.remove_resources(item_recipe.get_result_id(), input_quality, 1);
        double slowdown = 1.0 + (modules_count * constants_t::MODULE_SLOWDOWN_COEFFICIENT);
        ticks_left = static_cast<int>(std::round(item_recipe.get_base_time() * slowdown));

        is_working = true;
    }
}

bool recycle_machine::tick(inventory& inv)
{
    if (base_machine::tick())
    {
        inv.process_recycle_return(item_recipe.get_inputs(), input_quality);
        return true;
    }
    return false;
}
