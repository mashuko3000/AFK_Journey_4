//
// Created by mvideo on 17/03/26.
//

#ifndef CIPHERATTACK_VERNAMCIPHER_HPP
#define CIPHERATTACK_VERNAMCIPHER_HPP

#include"interfaces.hpp"

class VernamCipher : public ICipher
{
public:
    std::string encrypt(const std::string& text, const std::string& key) const override
    {
        std::string result = text;
        size_t textSize = text.size();
        size_t keySize = key.size();
        for(size_t i = 0; i < textSize; ++i)
        {
            result[i] = text[i] ^ key[i % keySize];
        }
        return result;
    }

    std::string decrypt(const std::string& cipher, const std::string& key) const override
    {
        return encrypt(cipher, key);
    }
};


#endif //CIPHERATTACK_VERNAMCIPHER_HPP
