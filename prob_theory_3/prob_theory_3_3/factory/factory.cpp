//
// Created by mvideo on 02/03/26.
//

#include "factory.hpp"

void factory::add_assembler(const recipe& r, quality_level_t q, int modules)
{
    assembly_machine machine(r, q);
    machine.set_modules(modules);
    assemblers.push_back(machine);
}

void factory::add_recycler(const recipe& r, quality_level_t q, int modules)
{
    recycle_machine machine(r, q);
    machine.set_modules(modules);
    recyclers.push_back(machine);
}

void factory::set_income_rate(int item_id, int rate)
{
    income_rates[item_id] = rate;
}

void factory::reset_factory()
{
    current_tick = 0;
    warehouse.clear();
    stats.reset();
    for (auto& machine : assemblers)
    {
        machine.reset_machine();
    }

    for (auto& machine : recyclers)
    {
        machine.reset_machine();
    }
}

void factory::step()
{
    current_tick++;
    stats.update_time(current_tick);
    for (auto const& [id, rate] : income_rates)
    {
        warehouse.add_resources(id, quality_level_t::COMMON, rate);
    }
    for (auto& machine : assemblers)
    {
        if (!machine.busy())
        {
            machine.try_start_production(warehouse);
        }
        machine.tick(warehouse, rng, stats, current_tick);
    }
    for (auto& machine : recyclers)
    {
        if (!machine.busy())
        {
            machine.try_start_recycling(warehouse);
        }
        machine.tick(warehouse);
    }
}

bool factory::is_simulation_complete() const
{
    return stats.is_goal_reached();
}

const production_stats& factory::get_final_stats() const
{
    return stats.get_stats();
}