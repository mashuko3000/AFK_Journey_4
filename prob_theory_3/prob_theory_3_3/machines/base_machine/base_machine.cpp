//
// Created by mvideo on 01/03/26.
//

#include "base_machine.hpp"
#include<stdexcept>

base_machine::base_machine()
        : modules_count(0), ticks_left(0), is_working(false) {}

void base_machine::set_modules(int count) {
    if (count < 0 || count > constants_t::MAX_MODULES)
    {
        throw std::invalid_argument("Invalid modules count");
    }
    modules_count = count;
}

bool base_machine::tick()
{
    if (!is_working)
    {
        return false;
    }

    if (ticks_left > 0)
    {
        ticks_left--;
    }

    if (ticks_left == 0)
    {
        is_working = false;
        return true;
    }

    return false;
}

void base_machine::reset_machine()
{
    ticks_left = 0;
    is_working = false;
}