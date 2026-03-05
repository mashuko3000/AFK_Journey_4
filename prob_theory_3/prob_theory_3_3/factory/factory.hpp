//
// Created by mvideo on 02/03/26.
//

#ifndef PROB_THEORY_3_3_FACTORY_HPP
#define PROB_THEORY_3_3_FACTORY_HPP

#include"base_machine/base_machine.hpp"
#include"recipe/recipe.hpp"
#include"inventory/inventory.hpp"
#include"quality_logic/quality_logic.hpp"
#include "stats_logic/stats_logic.hpp"
#include"assembley_machine/assembley_machine.hpp"
#include"recycle_machine/recycle_machine.hpp"

class factory {
private:
    int current_tick;

    inventory warehouse;
    random_generator rng;
    stats_logic stats;

    std::vector<assembly_machine> assemblers;
    std::vector<recycle_machine> recyclers;

    std::map<int, int> income_rates;

public:
    factory() : current_tick(0), rng(42){};

    factory(const factory&) = delete;
    factory& operator=(const factory&) = delete;

    factory(factory&&) noexcept = default;
    factory& operator=(factory&&) noexcept = default;

    ~factory() = default;

    void add_assembler(const recipe& r, quality_level_t q, int modules);
    void add_recycler(const recipe& r, quality_level_t q, int modules);
    void set_income_rate(int item_id, int rate);

    void reset_factory();

    void step();
    bool is_simulation_complete() const;
    int get_current_tick() const
    {
        return current_tick;
    }
    const production_stats& get_final_stats() const;
    int query_warehouse(int item_id, quality_level_t q) const;
};


#endif //PROB_THEORY_3_3_FACTORY_HPP
