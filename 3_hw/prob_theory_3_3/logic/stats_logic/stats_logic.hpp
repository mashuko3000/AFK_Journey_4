//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_3_STATS_LOGIC_HPP
#define PROB_THEORY_3_3_STATS_LOGIC_HPP

#include "types.hpp"

class stats_logic {
private:
    production_stats stats;
public:
    void log_production(quality_level_t quality, int current_tick);
    bool is_goal_reached() const;
    const production_stats& get_stats() const;
    int get_legendare_count() const {return stats.legendary_count;}
    void reset()
    {
        stats.legendary_count = 0;
        stats.total_ticks = 0;
        stats.rare_time.clear();
        stats.epic_time.clear();
        stats.legendary_times.clear();
    }
    void update_time(int tick) { stats.total_ticks = tick; }
};


#endif //PROB_THEORY_3_3_STATS_LOGIC_HPP
