//
// Created by mvideo on 17/03/26.
//

#ifndef EMAIL_NAIVEBAYES_HPP
#define EMAIL_NAIVEBAYES_HPP

#include"interfacesAndEntity.hpp"
#include<cmath>

class NaiveBayes : public IClassifier
{
    double alpha;
    size_t vocabSize = 0;
    std::unordered_map<Label, double> logPriorProb;
    std::unordered_map<Label, std::unordered_map<std::string_view, int>> globalWordCounts;
    std::unordered_map<Label, int> globalTotalWords;

public:
    NaiveBayes(double laplaceAlpha = 1.0) : alpha(laplaceAlpha){}
    ~NaiveBayes() override = default;
    void classify(const std::unordered_map<Label, int>& docsCount,
                          const std::unordered_map<Label, std::unordered_map<std::string_view, int>>& wordCounts,
                          const std::unordered_map<Label, int>& totalWords,
                          size_t vSize) override
    {
        vocabSize = vSize;
        globalWordCounts = wordCounts;
        globalTotalWords = totalWords;

        int totalDocs = 0;
        for(const auto& [lbl, count] : docsCount) totalDocs+=count;
        for(const auto& [lbl, count] : docsCount)
        {
            logPriorProb[lbl] = std::log(static_cast<double>(count)/totalDocs);
        }
    }
    /*
    Label predict(const std::vector<std::string_view>& tokens) const override
    {
        Label bestLabel = Label::HAM;
        double maxScore = - std::numeric_limits<double>::infinity();

        for(const auto&[label, prior] : logPriorProb)
        {
            double currentScore = prior;

            for(const auto& word : tokens)
            {
                int count = 0;
                auto it = globalWordCounts.at(label).find(word);
                if(it != globalWordCounts.at(label).end()) count = it->second;

                double prob = (count + alpha) / (globalTotalWords.at(label) + alpha * vocabSize);
                currentScore += std::log(prob);
            }

            if(currentScore > maxScore)
            {
                maxScore = currentScore;
                bestLabel = label;
            }
        }
        return bestLabel;
    }*/
    Label predict(const std::vector<std::string_view>& tokens) const override
    {
        double spamScore = logPriorProb.at(Label::SPAM);
        double hamScore = logPriorProb.at(Label::HAM);

        for(const auto& word : tokens)
        {
            int countSpam = 0;
            auto itSpam = globalWordCounts.at(Label::SPAM).find(word);
            if(itSpam != globalWordCounts.at(Label::SPAM).end()) countSpam = itSpam->second;
            spamScore += std::log((countSpam + alpha) / (globalTotalWords.at(Label::SPAM) + alpha * vocabSize));

            int countHam = 0;
            auto itHam = globalWordCounts.at(Label::HAM).find(word);
            if(itHam != globalWordCounts.at(Label::HAM).end()) countHam = itHam->second;
            hamScore += std::log((countHam + alpha) / (globalTotalWords.at(Label::HAM) + alpha * vocabSize));
        }
        double spamThreshold = 2.0;

        if (spamScore > (hamScore + spamThreshold))
        {
            return Label::SPAM;
        }
        else
        {
            return Label::HAM;
        }
    }
};

#endif //EMAIL_NAIVEBAYES_HPP
