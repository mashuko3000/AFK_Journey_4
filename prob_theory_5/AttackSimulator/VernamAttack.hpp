//
// Created by mvideo on 16/03/26.
//

#ifndef ATTACKSIMULATOR_VERNAMATTACK_HPP
#define ATTACKSIMULATOR_VERNAMATTACK_HPP

#include"interfaces.hpp"

#include<random>
#include<memory>

class VernamCipher : public ICipher
{
private:
    std::uint16_t key;
public:
    explicit VernamCipher(std::uint16_t k) : key(k) {}

    std::uint16_t encrypt(std::uint16_t m) const override
    {
        return m ^ key;
    }
};

class Random16BitSetGenerator : public ISetGenerator
{
private:
    std::mt19937 gen;
    std::uniform_int_distribution<uint16_t> dist;

public:
    Random16BitSetGenerator() : gen(std::random_device{}()), dist(0, 65535) {}

    std::unordered_set<uint16_t> generate(size_t k) override
    {
        std::unordered_set<uint16_t> s;
        while (s.size() < k)
        {
            s.insert(dist(gen));
        }
        return s;
    }
};

class BirthdayAttackSimulator : public ICollisionSimulator
{
private:
    std::shared_ptr<ISetGenerator> generator;
    std::shared_ptr<ICipher> cipher;

public:
    BirthdayAttackSimulator(std::shared_ptr<ISetGenerator> g, std::shared_ptr<ICipher> c)
            : generator(std::move(g)), cipher(std::move(c)) {}

    double runSimulator(size_t k, size_t trials) override
    {
        size_t success_count = 0;

        for (size_t i = 0; i < trials; ++i)
        {
            auto X1 = generator->generate(k);
            auto X2 = generator->generate(k);

            std::unordered_set<uint16_t> encrypted_X1;
            for (uint16_t x : X1)
            {
                encrypted_X1.insert(cipher->encrypt(x));
            }

            bool collision_found = false;
            for (uint16_t x : X2)
            {
                if (encrypted_X1.count(cipher->encrypt(x)))
                {
                    collision_found = true;
                    break;
                }
            }

            if (collision_found)
            {
                success_count++;
            }
        }

        return static_cast<double>(success_count) / static_cast<double>(trials);
    }
};

#endif //ATTACKSIMULATOR_VERNAMATTACK_HPP
