//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_3_BASE_MACHINE_HPP
#define PROB_THEORY_3_3_BASE_MACHINE_HPP

#include"constants.hpp"

class base_machine
{
protected:
    int modules_count;
    int ticks_left;
    bool is_working;

public:
    base_machine();
    virtual ~base_machine() = default;
    void set_modules(int count);
    virtual bool tick();
    bool busy() const {return is_working;}
    int get_modules() const {return modules_count;}
    void reset_machine();
};


#endif //PROB_THEORY_3_3_BASE_MACHINE_HPP
