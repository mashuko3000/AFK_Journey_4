//
// Created by mvideo on 17/03/26.
//

#ifndef EMAIL_PIPELINE_HPP
#define EMAIL_PIPELINE_HPP

#include"interfacesAndEntity.hpp"
#include"NLP.hpp"

ThreadStats processChunk(
        char* start,
        char* end,
        NLPProcessor& nlp,
        double trainRatio
        )
{
    ThreadStats stats;
    std::hash<std::string_view> hasher;

    nlp.preprocessInPlace(start, end);

    std::string_view chunk(start, end - start);
    size_t lineStart = 0;

    while(lineStart < chunk.size())
    {
        size_t lineEnd = chunk.find('\n', lineStart);
        if(lineEnd == std::string_view::npos) lineEnd = chunk.size();

        std::string_view line = chunk.substr(lineStart, lineEnd - lineStart);
        lineStart = lineEnd + 1;

        size_t tabPos = line.find('\t');
        if (tabPos == std::string_view::npos) continue;

        std::string_view labelStr = line.substr(0, tabPos);
        std::string_view text = line.substr(tabPos + 1);
        Label label = (labelStr == "spam") ? Label::SPAM : Label::HAM;

        auto tokens = nlp.tokenize(text);

        if ((hasher(line) % 100) < (trainRatio * 100))
        {
            stats.docsCount[label]++;
            for (auto t : tokens)
            {
                stats.wordCounts[label][t]++;
                stats.totalWords[label]++;
            }
        }
        else
        {
            stats.testDocs.push_back({label, tokens});
        }
    }
    return stats;
}

#endif //EMAIL_PIPELINE_HPP
