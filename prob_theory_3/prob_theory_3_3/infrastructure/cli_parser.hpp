//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_3_CLI_PARSER_HPP
#define PROB_THEORY_3_3_CLI_PARSER_HPP

#include"types.hpp"

class cli_parser {
public:
    static config_t parse(int argc, char* argv[]);
    static void print_help();
};


#endif //PROB_THEORY_3_3_CLI_PARSER_HPP
