//
// Created by mvideo on 09/03/26.
//

#include "PontWalking.hpp"

#include<fstream>
#include <sstream>
#include <iostream>
#include<iomanip>

void PointWalking::loadConfig(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        throw std::runtime_error("Cannot open the fle");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    settings.probPositiveP = std::stod(findValueInJson(content, "p"));
    settings.probNegativeQ = 1.0 - settings.probPositiveP;
    settings.stepPositiveS = std::stod(findValueInJson(content, "s_plus"));
    settings.stepNegativeS = std::stod(findValueInJson(content, "s_minus"));
    settings.maxStepsToReturnN = std::stoull(findValueInJson(content, "N"));
    settings.totalTrials = std::stoul(findValueInJson(content, "total_trials"));
}
void PointWalking::run()
{
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    results.totalRuns = 0;
    results.noReturnCount = 0;
    results.returnCounts.clear();

    for(std::uint32_t i = 0; i < settings.totalTrials; ++i)
    {
        double currentPosition = 0.0;
        bool returned = false;
        for(std::uint64_t step = 1; step <= settings.maxStepsToReturnN; ++step)
        {
            if(dist(gen) < settings.probPositiveP)
            {
                currentPosition += settings.stepPositiveS;
            }
            else
            {
                currentPosition -= settings.stepNegativeS;
            }
            if(std::abs(currentPosition) < 1e-10)
            {
                results.returnCounts[step]++;
                returned = true;
                break;
            }
        }
        if (!returned) results.noReturnCount++;
        results.totalRuns++;
    }
}
void PointWalking::printStatistics(std::ostream& out = std::cout) const
{
    if (results.totalRuns == 0)
    {
        throw std::runtime_error("Simulation wasnt processed");
    }

    double noReturnProb = static_cast<double>(results.noReturnCount) / results.totalRuns;

    out << "\n === RESULT SIMULATION FIRST RETURN === \n";
    out << "Parameters : \n";
    out << "p      | " << settings.probPositiveP << "\n";
    out << "q      | " << settings.probNegativeQ << "\n";
    out << "s+     | " << settings.stepPositiveS << "\n";
    out << "s-     | " << settings.stepNegativeS << "\n";
    out << "max N  | " << settings.maxStepsToReturnN << "\n";
    out << "trials | " << settings.totalTrials << "\n\n";

    out << "Amount of Trajectories was : " << results.totalRuns << "\n";
    out << "FAILED out of all          : " << results.noReturnCount
            << "  [" << std::fixed << std::setprecision(4) << noReturnProb << "]\n\n";

    if(results.returnCounts.empty())
    {
        out << "There wasnt returns";
    }

    out << "N              Count     Probability\n";
    out << "------------------------------------------------------------\n";

    for (const auto& [n, cnt] : results.returnCounts) {
        double p = static_cast<double>(cnt) / results.totalRuns;
        out << std::setw(12) << n
            << std::setw(12) << cnt
            << std::setw(18) << std::fixed << std::setprecision(8) << p
            << "\n";
    }

    out << "------------------------------------------------------------\n";
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

std::string PointWalking::findValueInJson(const std::string& json, const std::string& key)
{
    std::string searchKey = "\"" + key + "\":";
    size_t keyPos = json.find(searchKey);

    if (keyPos == std::string::npos)
    {
        throw std::runtime_error("Key not found in JSON: " + key);
    }

    size_t valueStart = json.find_first_not_of(" \t\n\r\f\v", keyPos + searchKey.length());
    if (valueStart == std::string::npos)
    {
        throw std::runtime_error("Invalid JSON after key: " + key);
    }

    char firstChar = json[valueStart];
    size_t valueEnd;
    if(firstChar == '"')
    {
        valueStart++;
        valueEnd = json.find('"', valueStart);
        if (valueEnd == std::string::npos)
        {
            throw std::runtime_error("Unclosed string for key: " + key);
        }
        else
        {
            valueEnd = json.find_first_of(",}", valueStart);
            if (valueEnd == std::string::npos)
            {
                throw std::runtime_error("No end of value for key: " + key);
            }
        }
    }
    std::string value = trim(json.substr(valueStart, valueEnd - valueStart));
    return value;
}
double PointWalking::parseDouble(const std::string& content, const std::string& key)
{
    std::string valueStr = findValueInJson(content, key);
    try
    {
        return std::stod(valueStr);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to parse double for key: " + key + " (" + valueStr + ")");
    }
}
std::uint64_t PointWalking::parseUint64(const std::string& content, const std::string& key)
{
    std::string valueStr = findValueInJson(content, key);
    try
    {
        return std::stoull(valueStr);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse uint64_t for key: " + key + " (" + valueStr + ")");
    }
}
