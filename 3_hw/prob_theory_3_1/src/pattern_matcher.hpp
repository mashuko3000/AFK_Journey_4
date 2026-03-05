//
// Created by mvideo on 28/02/26.
//

#ifndef PROB_THEORY_3_1_PATTERN_MATCHER_HPP
#define PROB_THEORY_3_1_PATTERN_MATCHER_HPP

#include"cluster.hpp"

class pattern_matcher {
public:
    static match_result are_connectable(const cluster& left,
                                const cluster& right,
                                const string& pattern);
};


#endif //PROB_THEORY_3_1_PATTERN_MATCHER_HPP
