//
// Created by mvideo on 28/02/26.
//

#ifndef PROB_THEORY_3_1_CLUSTER_HPP
#define PROB_THEORY_3_1_CLUSTER_HPP

#include "types.hpp"
#include "alphabet.hpp"

class cluster {
private:
    string data;
    size_t n;

public:
    explicit cluster(size_t size);

    void fill(alphabet& alpha);
    string get_content() const;
    string get_suffix(size_t k) const;
    string get_prefix(size_t k) const;

    size_t size() const {return n;}
    const string& content() const;
};


#endif //PROB_THEORY_3_1_CLUSTER_HPP
