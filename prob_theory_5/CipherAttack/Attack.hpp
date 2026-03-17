//
// Created by mvideo on 17/03/26.
//

#ifndef CIPHERATTACK_ATTACK_HPP
#define CIPHERATTACK_ATTACK_HPP

#include "interfaces.hpp"
#include "StatisticalAttack.hpp"

class VigenereAttack : public IStatisticalAttack
{
private:
    std::string ciphertext;
    StatisticalAnalyzer analyzer;
    const double target = 0.065;

public:
    explicit VigenereAttack(const std::string& ct) : ciphertext(ct){}

    void executeAttack() override
    {
        std::cout << "===STATS ATTACK ON VIGENERE CIPHER===" << std::endl;
        int bestKeyLength = 1;
        double closestICDiff = 1.0;

        for(int L = 1; L <= 10; ++L)
        {
            double avgIC = 0.0;
            for (int i = 0; i < L; ++i)
            {
                std::string col = analyzer.extractColumn(ciphertext, L, i);
                avgIC += analyzer.calculateIC(col);
            }

            avgIC /= L;

            std::cout << "Supposed length of key : " << L
            << " | AVG IC : " << avgIC << std::endl;
            if(std::abs(avgIC - target) < closestICDiff)
            {
                closestICDiff = std::abs(avgIC - target);
                bestKeyLength = L;
            }
        }
        std::cout << "Static more probably length of key : " << bestKeyLength << std::endl;
    }
};

class VernamAttack : public IStatisticalAttack
{
private:
    std::string cipher1;
    std::string cipher2;

public:
    VernamAttack(const std::string& c1, const std::string& c2) : cipher1(c1), cipher2(c2) {}

    void executeAttack() override
    {
        std::cout << "===STATS ATTACK ON VERNAM CIPHER===" << std::endl;
        size_t len = std::min(cipher1.length(), cipher2.length());

        std::cout << "XOR of two ciphertexts : ";
        std::vector<unsigned char> xorStream(len);
        for (size_t i = 0; i < len; ++i)
        {
            xorStream[i] = cipher1[i] ^ cipher2[i];
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)xorStream[i] << " ";
        }
        std::cout << std::dec << "\n\nStatistical search of space : " << std::endl;
        for (size_t i = 0; i < len; ++i)
        {
            unsigned char testChar = xorStream[i] ^ ' ';
            if (isalpha(testChar))
            {
                std::cout << "Position " << i << ": Letter '"
                          << (char)testChar << "' and space.\n";
            }
        }
    }
};

#endif //CIPHERATTACK_ATTACK_HPP
