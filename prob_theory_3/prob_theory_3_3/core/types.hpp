//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_3_TYPES_HPP
#define PROB_THEORY_3_3_TYPES_HPP

#include<vector>

enum class quality_level_t : int {
    COMMON = 1,
    UNCOMMON = 2,
    RARE = 3,
    EPIC = 4,
    LEGENDARY = 5
};

struct ingredients
{
    int component_id;
    int count;
};

struct production_stats
{
    std::vector<int> rare_time;
    std::vector<int> epic_time;
    std::vector<int> legendary_times;
    int legendary_count = 0;
    int total_ticks = 0;
};
struct config_t
{
    int target_legendary = 25;
    int modules_count = 0;
    int income_rate = 10;
    int seed = 42;
};

#endif //PROB_THEORY_3_3_TYPES_HPP
