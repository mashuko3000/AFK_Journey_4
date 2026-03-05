//
// Created by mvideo on 28/02/26.
//

#ifndef PROB_THEORY_3_1_TYPES_HPP
#define PROB_THEORY_3_1_TYPES_HPP

#include<string>
#include<vector>

using string = std::string;

struct match_result
{
    bool connected = false;
    string left_match;
    string right_match;
    string left_rem;
    string right_rem;
};

#endif //PROB_THEORY_3_1_TYPES_HPP
