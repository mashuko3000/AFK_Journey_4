//
// Created by mvideo on 17/03/26.
//

#ifndef EMAIL_STANDARDEVALUATOR_HPP
#define EMAIL_STANDARDEVALUATOR_HPP

#include"interfacesAndEntity.hpp"

class StandardEvaluator : public IEvaluator
{
public:
    MetricResult evaluate(const std::vector<std::pair<Label, Label>>& actualVsPredicted) const override
    {
        int TP = 0, TN = 0, FP = 0, FN = 0;

        int total = actualVsPredicted.size();

        if (total == 0) return {};

        for (const auto& [actual, predicted] : actualVsPredicted)
        {
            if (actual == Label::SPAM && predicted == Label::SPAM) TP++;
            else if (actual == Label::HAM && predicted == Label::HAM) TN++;
            else if (actual == Label::HAM && predicted == Label::SPAM) FP++;
            else if (actual == Label::SPAM && predicted == Label::HAM) FN++;
        }

        MetricResult m;
        m.accuracy = static_cast<double>(TP + TN) / (TP + TN + FP + FN);
        m.precision = (TP + FP == 0) ? 0 : static_cast<double>(TP) / (TP + FP);
        m.recall = (TP + FN == 0) ? 0 : static_cast<double>(TP) / (TP + FN);
        m.f1Score = (m.precision + m.recall == 0) ? 0 : 2 * (m.precision * m.recall) / (m.precision + m.recall);
        return m;
    }
};

#endif //EMAIL_STANDARDEVALUATOR_HPP
