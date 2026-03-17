//
// Created by mvideo on 17/03/26.
//

#ifndef CIPHERATTACK_TYPES_HPP
#define CIPHERATTACK_TYPES_HPP

#include<map>
#include<string>

struct Probability
{
    std::map<std::string, double> texts;
    std::map<std::string, double> keys;
};

#endif //CIPHERATTACK_TYPES_HPP
