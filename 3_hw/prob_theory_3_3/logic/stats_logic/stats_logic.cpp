//
// Created by mvideo on 01/03/26.
//

#include "stats_logic.hpp"
#include "constants.hpp"

void stats_logic::log_production(
        quality_level_t quality,
        int current_tick)
{
    switch(quality)
    {
        case quality_level_t::RARE :
            stats.rare_time.push_back(current_tick);
            break;
        case quality_level_t::EPIC :
            stats.epic_time.push_back(current_tick);
            break;
        case quality_level_t::LEGENDARY :
            stats.legendary_times.push_back(current_tick);
            stats.legendary_count++;
            break;
        default:
            break;
    }
}
bool stats_logic::is_goal_reached() const
{
    return stats.legendary_count >= constants_t::TARGET_LEGENDARY_COUNT;
}
const production_stats& stats_logic::get_stats() const
{
    return stats;
}