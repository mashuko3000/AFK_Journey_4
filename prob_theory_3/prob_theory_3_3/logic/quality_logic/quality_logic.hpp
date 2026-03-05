//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_3_QUALITY_LOGIC_HPP
#define PROB_THEORY_3_3_QUALITY_LOGIC_HPP

#include "types.hpp"
#include "generator/random_generator.hpp"

class quality_logic
{
private:
    static int clamp_quality(int quality_value);
public:
    static quality_level_t calculate_result_quality(
            quality_level_t base_quality,
            int modules_count,
            random_generator& rng);
};


#endif //PROB_THEORY_3_3_QUALITY_LOGIC_HPP
