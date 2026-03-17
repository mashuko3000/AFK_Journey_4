//
// Created by mvideo on 16/03/26.
//

#ifndef ATTACKSIMULATOR_INTERFACES_HPP
#define ATTACKSIMULATOR_INTERFACES_HPP

#include<cstdint>
#include<unordered_set>

class ICipher
{
public:
    virtual ~ICipher() = default;
    virtual std::uint16_t encrypt(std::uint16_t m) const = 0;
};

class ISetGenerator
{
public:
    virtual ~ISetGenerator() = default;
    virtual std::unordered_set<std::uint16_t> generate(size_t k) = 0;
};

class ICollisionSimulator
{
public:
    virtual ~ICollisionSimulator() = default;
    virtual double runSimulator(size_t k, size_t trials) = 0;
};

#endif //ATTACKSIMULATOR_INTERFACES_HPP
