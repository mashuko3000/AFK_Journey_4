//
// Created by mvideo on 01/03/26.
//

#include "components.hpp"
#include<stdexcept>
components::components(int id, std::string name)
{
    if (id < 0)
    {
        throw std::invalid_argument("component ID cannot be negative");
    }
    if (name.empty())
    {
        throw std::invalid_argument("component name cannot be empty");
    }

    this->id = id;
    this->name = name;
}
int components::get_id() const
{
    return id;
}
const std::string& components::get_name() const
{
    return name;
}