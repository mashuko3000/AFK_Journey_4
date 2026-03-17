//
// Created by mvideo on 17/03/26.
//

#ifndef EMAIL_RUNAPP_HPP
#define EMAIL_RUNAPP_HPP

#include<string>
#include<unordered_map>
#include"Memory.hpp"
#include"NaiveBayes.hpp"
#include"NLP.hpp"
#include"PipeLine.hpp"
#include"StandardEvaluator.hpp"
#include<thread>
#include<future>
#include<iostream>

void runApp(const std::string& filepath)
{
    MutableMemoryMappedFile mmapFile(filepath);

    NLPProcessor nlp({"the","and","to","of","in","a","is","it","you","that","for","on","with","as","be","this","are","have","but","not"});
    NaiveBayes classifier(1.0);
    StandardEvaluator evaluator;

    unsigned int numThreads = std::thread::hardware_concurrency();
    std::vector<std::future<ThreadStats>> futures;

    size_t chunkSize = mmapFile.getSize() / numThreads;
    char* currentPos = mmapFile.begin();

    std::cout << "[*] Run on " << numThreads << " threads...\n";

    for (unsigned int i = 0; i < numThreads; ++i)
    {
        if (currentPos >= mmapFile.end())
        {
            break;
        }

        char* endPos = currentPos + chunkSize;
        if (endPos > mmapFile.end() || i == numThreads - 1)
        {
            endPos = mmapFile.end();
        }

        if (endPos < mmapFile.end())
        {
            while (endPos < mmapFile.end() && *endPos != '\n') {
                endPos++;
            }
        }

        futures.push_back(std::async(std::launch::async, processChunk, currentPos, endPos, std::ref(nlp), 0.7));

        currentPos = endPos + 1;
    }

    std::unordered_map<Label, int> globalDocsCount;
    std::unordered_map<Label, std::unordered_map<std::string_view, int>> globalWordCounts;
    std::unordered_map<Label, int> globalTotalWords;
    std::unordered_set<std::string_view> vocabulary;
    std::vector<Document> testSet;

    for (auto& fut : futures)
    {
        auto stats = fut.get();
        for (const auto& [lbl, cnt] : stats.docsCount) globalDocsCount[lbl] += cnt;
        for (const auto& [lbl, cnt] : stats.totalWords) globalTotalWords[lbl] += cnt;
        for (const auto& [lbl, words] : stats.wordCounts)
        {
            for (const auto& [w, cnt] : words)
            {
                globalWordCounts[lbl][w] += cnt;
                vocabulary.insert(w);
            }
        }
        testSet.insert(testSet.end(),
                       std::make_move_iterator(stats.testDocs.begin()),
                       std::make_move_iterator(stats.testDocs.end()));
    }

    classifier.classify(globalDocsCount,
                        globalWordCounts,
                        globalTotalWords,
                        vocabulary.size());

    std::vector<std::pair<Label, Label>> results;
    for (const auto& doc : testSet)
    {
        results.push_back({doc.m_label, classifier.predict(doc.tokens)});
    }

    MetricResult m = evaluator.evaluate(results);

    std::cout << "--- Results ---\n";
    std::cout << "Accuracy:  " << m.accuracy << "\n";
    std::cout << "Precision: " << m.precision << "\n";
    std::cout << "Recall:    " << m.recall << "\n";
    std::cout << "F1 Score:  " << m.f1Score << "\n";
}
#endif //EMAIL_RUNAPP_HPP
