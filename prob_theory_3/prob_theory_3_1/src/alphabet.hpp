//
// Created by mvideo on 28/02/26.
//

#ifndef PROB_THEORY_3_1_ALPHABET_HPP
#define PROB_THEORY_3_1_ALPHABET_HPP

#include "types.hpp"
#include "distribution_model.hpp"

using namespace std;

class alphabet{
private:
    string symbols;
    i_distribution_model* distribution;
public:
    alphabet(const string& characters, i_distribution_model* model);

    ~alphabet() noexcept;

    alphabet(const alphabet& source);
    alphabet& operator=(const alphabet& source);

    alphabet(alphabet&& source) noexcept;
    alphabet& operator=(alphabet&& source) noexcept;

    char generate_symbol();
    size_t size() const {return symbols.size();}
    bool contains(char s) const;

    friend void swap(alphabet& first, alphabet& second) noexcept;
};


#endif //PROB_THEORY_3_1_ALPHABET_HPP
