//
// Created by mvideo on 01/03/26.
//

#include "quality_logic.hpp"
#include <stdexcept>
int quality_logic::clamp_quality(int quality_value)
{
    if(quality_value > 5) return 5;
    if(quality_value < 1) return 1;
    return quality_value;
}
quality_level_t quality_logic::calculate_result_quality(
        quality_level_t base_quality,
        int modules_count,
        random_generator &rng)
{
    int current_level = static_cast<int>(base_quality);
    if(modules_count <= 0)
    {
        return base_quality;
    }
    int upgrade_steps = rng.roll_update(modules_count);
    int new_level_value = clamp_quality(current_level + upgrade_steps);
    return static_cast<quality_level_t>(new_level_value);
}