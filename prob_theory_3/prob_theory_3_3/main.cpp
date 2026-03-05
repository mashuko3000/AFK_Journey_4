#include <iostream>

#include "infrastructure/cli_parser.hpp"
#include "factory/factory.hpp"
#include "models/components/components.hpp"
#include "models/recipe/recipe.hpp"
#include "core/types.hpp"

int main(int argc, char* argv[]) {
    config_t config = cli_parser::parse(argc, argv);
    components iron_ore(1, "Iron Ore");
    components iron_plate(2, "Iron Plate");

    std::vector<ingredients> plate_ingredients = { {1, 2} };
    recipe plate_recipe(2, plate_ingredients, 50);

    factory my_factory;

    my_factory.set_income_rate(1, config.income_rate);
    my_factory.add_assembler(plate_recipe, quality_level_t::COMMON, config.modules_count);

    for (int q = 1; q <= static_cast<int>(quality_level_t::LEGENDARY); ++q)
    {
        my_factory.add_recycler(plate_recipe, static_cast<quality_level_t>(q), config.modules_count);
    }

    std::cout << "Starting simulation with " << config.modules_count << " modules..." << std::endl;
    std::cout << "Target: " << config.target_legendary << " legendary items." << std::endl;

    while (!my_factory.is_simulation_complete()) {
        my_factory.step();
        if (my_factory.get_current_tick() % 10000 == 0) {
            std::cout << "Tick: " << my_factory.get_current_tick()
                      << " | Found: " << my_factory.get_final_stats().legendary_count << std::endl;
        }
    }

    const auto& results = my_factory.get_final_stats();
    std::cout << "\n--- SIMULATION COMPLETE ---" << std::endl;
    std::cout << "Total time (ticks): " << results.total_ticks << std::endl;
    std::cout << "Legendary items produced: " << results.legendary_count << std::endl;

    if (!results.legendary_times.empty()) {
        std::cout << "First legendary at tick: " << results.legendary_times[0] << std::endl;
        std::cout << "Last legendary at tick: " << results.legendary_times.back() << std::endl;
    }

    return 0;
}
