//
// Created by mvideo on 17/03/26.
//

#ifndef CIPHERATTACK_INTERFACES_HPP
#define CIPHERATTACK_INTERFACES_HPP

#include<cstdint>
#include<string>

class ICipher
{
public:
    virtual ~ICipher() = default;
    virtual std::string encrypt(const std::string& text, const std::string& key) const = 0;
    virtual std::string decrypt(const std::string& cipher, const std::string& key) const = 0;
};

class IAttacker
{
public:
    virtual ~IAttacker() = default;
    virtual void attack(const std::string& cipherText) = 0;
};

class IStatisticalAttack
{
public:
    virtual ~IStatisticalAttack() = default;
    virtual void executeAttack() = 0;
};


#endif //CIPHERATTACK_INTERFACES_HPP
