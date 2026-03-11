//
// Created by mvideo on 09/03/26.
//

#ifndef PROB_THEORY_HUB_PONTWALKING_HPP
#define PROB_THEORY_HUB_PONTWALKING_HPP

#include<cstdint>
#include<string>
#include<random>
#include <map>
#include <ostream>

struct config
{
    double probPositiveP = 0.0;
    double probNegativeQ = 0.0;
    double stepPositiveS = 0.0;
    double stepNegativeS = 0.0;
    std::uint64_t maxStepsToReturnN = 0;
    std::uint32_t totalTrials = 0;
};

struct resultSimulation
{
    std::uint64_t totalRuns = 0;
    std::uint64_t noReturnCount = 0;
    std::map<std::uint64_t, std::uint64_t> returnCounts;
};

class PointWalking
{
private:
    config settings;
    resultSimulation results;
    std::mt19937_64 gen{std::random_device{}()};

public:
    PointWalking() = default;
    ~PointWalking() = default;

    void loadConfig(const std::string& filename);
    void run();
    void printStatistics(std::ostream& out) const;

private:
    std::string findValueInJson(const std::string& json, const std::string& key);
    double parseDouble(const std::string& content, const std::string& key);
    std::uint64_t parseUint64(const std::string& content, const std::string& key);
};


#endif //PROB_THEORY_HUB_PONTWALKING_HPP
