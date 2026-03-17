//
// Created by mvideo on 17/03/26.
//

#ifndef EMAIL_NLP_HPP
#define EMAIL_NLP_HPP

#include<unordered_set>
#include<string_view>
#include<ctype.h>
#include<vector>

class NLPProcessor
{
private:
    std::unordered_set<std::string_view> stopWords;

public:
    NLPProcessor(std::unordered_set<std::string_view> sw) : stopWords(std::move(sw))
    {}

    void preprocessInPlace(char* b, char* e)
    {
        for(char* p = b; p < e; ++p)
        {
            unsigned char c = static_cast<unsigned char>(*p);
            if(!isalpha(c) && c != '\n' && c != '\t')
            {
                *p = ' ';
            }
            else
            {
                *p = tolower(c);
            }
        }
    }

    std::vector<std::string_view> tokenize(std::string_view text)
    {
        std::vector<std::string_view> tokens;
        size_t start = text.find_first_not_of(' ');

        while(start != std::string_view::npos)
        {
            size_t end = text.find(' ', start);
            std::string_view word = text.substr(start, end - start);
            if(word.length() > 1 && stopWords.find(word) == stopWords.end())
            {
                tokens.push_back(word);
            }
            start = text.find_first_not_of(' ', end);
        }

        return tokens;
    }
};

#endif //EMAIL_NLP_HPP
