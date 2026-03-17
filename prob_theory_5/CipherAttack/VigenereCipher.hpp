//
// Created by mvideo on 17/03/26.
//

#ifndef CIPHERATTACK_VIGENERECIPHER_HPP
#define CIPHERATTACK_VIGENERECIPHER_HPP

#include"interfaces.hpp"

class VigenereCipher : public ICipher
{
public:
    ~VigenereCipher() override = default;
    std::string encrypt(const std::string& text, const std::string& key) const override
    {
        std::string result = "";
        size_t textSize = text.size();
        size_t keySize = key.size();
        for(size_t i = 0, j = 0; i < textSize; ++i)
        {
            if(isalpha(text[i]))
            {
                char base = islower(text[i]) ? 'a' : 'A';
                char keyBase = islower(key[j & keySize]) ? 'a' : 'A';
                int shift = key[j % keySize] - keyBase;
                result += (text[i] - base + shift) % 26 + base;
                j++;
            }
            else
            {
                result += text[i];
            }
        }
        return result;
    }

    std::string decrypt(const std::string& cipher, const std::string& key) const override
    {
        std::string result = "";
        size_t cipherSize = cipher.size();
        size_t keySize = key.size();
        for(size_t i = 0, j = 0; i < cipherSize; ++i)
        {
            if(isalpha(cipher[i]))
            {
                char base = islower(cipher[i]) ? 'a' : 'A';
                char keyBase = islower(key[j % keySize]) ? 'a' : 'A';
                int shift = key[j % keySize] - keyBase;
                result += (cipher[i] - base - shift + 26) % 26 + base;
                j++;
            }
            else
            {
                result += cipher[i];
            }
        }
        return result;
    }
};

#endif //CIPHERATTACK_VIGENERECIPHER_HPP
