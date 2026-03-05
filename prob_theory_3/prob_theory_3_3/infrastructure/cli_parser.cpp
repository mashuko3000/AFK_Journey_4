//
// Created by mvideo on 01/03/26.
//

#include "cli_parser.hpp"

#include <string>
#include<iostream>

config_t cli_parser::parse(int argc, char* argv[])
{
    config_t config;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h")
        {
            print_help();
            exit(0);
        }
        else if (arg == "--target" && i + 1 < argc)
        {
            config.target_legendary = std::stoi(argv[++i]);
        }
        else if (arg == "--modules" && i + 1 < argc)
        {
            config.modules_count = std::stoi(argv[++i]);
        }
        else if (arg == "--income" && i + 1 < argc)
        {
            config.income_rate = std::stoi(argv[++i]);
        }
        else if (arg == "--seed" && i + 1 < argc)
        {
            config.seed = std::stoi(argv[++i]);
        }
    }

    return config;
}

void cli_parser::print_help()
{
    std::cout << "Factorio Quality Simulator\n"
              << "Usage: ./simulation [options]\n"
              << "Options:\n"
              << "  --target N   Set target legendary count (default: 25)\n"
              << "  --modules N  Set number of quality modules 0-4 (default: 0)\n"
              << "  --income N   Set resource income per tick (default: 10)\n"
              << "  --seed N     Set random seed (default: 42)\n";
}
