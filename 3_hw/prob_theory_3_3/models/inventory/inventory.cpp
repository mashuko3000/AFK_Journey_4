//
// Created by mvideo on 01/03/26.
//

#include "inventory.hpp"


void inventory::add_resources(int item_id, quality_level_t quality, int count)
{
    if (storage.find(item_id) == storage.end())
    {
        storage[item_id] = std::vector<int>(6, 0);
    }
    storage[item_id][static_cast<int>(quality)] += count;
}
void inventory::remove_resources(int item_id, quality_level_t quality, int count)
{
    if(has_enough(item_id, quality, count))
    {
        storage[item_id][static_cast<int>(quality)] -= count;
    }
}
bool inventory::has_enough(int item_id, quality_level_t quality, int count) const
{
    auto it = storage.find(item_id);
    if (it == storage.end()) return false;
    return it->second[static_cast<int>(quality)] >= count;
}
bool inventory::can_afford_recipe(const std::vector<ingredients>& recipe, quality_level_t quality) const
{
    for(const auto& ing : recipe)
    {
        if(!has_enough(ing.component_id, quality, ing.count)) return false;
    }
    return true;
}
void inventory::consume_recipe(const std::vector<ingredients>& recipe, quality_level_t quality)
{
    for(const auto& ing : recipe)
    {
        remove_resources(ing.component_id, quality, ing.count);
    }
}
void inventory::process_recycle_return(const std::vector<ingredients>& original_recipe, quality_level_t quality)
{
    for(const auto& ing : original_recipe)
    {
        int return_count = static_cast<int>(ing.count * constants_t::RECYCLE_RETURN_RATE);
        if(return_count > 0)
        {
            add_resources(ing.component_id, quality, return_count);
        }
    }
}
int inventory::get_count(int item_id, quality_level_t quality) const
{
    auto it = storage.find(item_id);
    if (it == storage.end()) return 0;
    return it -> second[static_cast<int>(quality)];
}
void inventory::clear()
{
    storage.clear();
}