//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_3_COMPONENTS_HPP
#define PROB_THEORY_3_3_COMPONENTS_HPP

#include<string>

class components {
private:
    int id;
    std::string name;
public:
    components(int is, std::string name);
    int get_id() const;
    const std::string& get_name() const;
};


#endif //PROB_THEORY_3_3_COMPONENTS_HPP
