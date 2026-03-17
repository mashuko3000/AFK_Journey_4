//
// Created by mvideo on 17/03/26.
//

#ifndef EMAIL_INTERFACESANDENTITY_HPP
#define EMAIL_INTERFACESANDENTITY_HPP

#include<string>
#include<vector>
#include <set>
#include <unordered_map>

enum class Label{SPAM, HAM, UNKNOWN};

struct Document
{
    Label m_label;
    std::vector<std::string_view> tokens;
};

struct ThreadStats
{
    std::unordered_map<Label, int> docsCount;
    std::unordered_map<Label, std::unordered_map<std::string_view, int>> wordCounts;
    std::unordered_map<Label, int> totalWords;
    std::vector<Document> testDocs;
};

struct MetricResult
{
    double accuracy = 0.0;
    double precision = 0.0;
    double recall = 0.0;
    double f1Score = 0.0;
};

class IPreprocessor
{
public:
    virtual ~IPreprocessor() = default;
    virtual void process(Document doc) const = 0;
};

class IClassifier
{
public:
    virtual ~IClassifier() = default;
    virtual void classify(const std::unordered_map<Label, int>& docsCount,
                          const std::unordered_map<Label, std::unordered_map<std::string_view, int>>& wordCounts,
                          const std::unordered_map<Label, int>& totalWords,
                          size_t vocabSize) = 0;
    virtual Label predict(const std::vector<std::string_view>& tokens) const = 0;
};

class IEvaluator
{
public:
    virtual ~IEvaluator() = default;
    virtual MetricResult evaluate(const std::vector<std::pair<Label, Label>>& actualVSPredicted) const = 0;
};
#endif //EMAIL_INTERFACESANDENTITY_HPP
