//
// Created by mvideo on 08/03/26.
//

#ifndef PROB_THEORY_HUB_HISTORYLOGGER_HPP
#define PROB_THEORY_HUB_HISTORYLOGGER_HPP

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "RandomWalking.hpp"

class HistoryLogger
{
public:
    static bool saveHistoryToFile(const QString& fileName, const simulationSummary& summary)
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
            return false;
        }

        QTextStream out(&file);
        out << "[Simulation Session: " << QDateTime::currentDateTime().toString() << " ]\n";
        out << "Total Trials: " << summary.totalSimulation << "\n";
        out << "Empirical Probability: " << summary.empiricalProbability << "\n";
        out << "Last " << summary.history.size() << " results:\n";

        for (const auto& trial : summary.history) {
            out << "Trial ID: " << trial.id
                << " | Crossings: " << trial.crossingCount
                << " | Match: " << (trial.isMatch ? "Yes" : "No") << "\n";
        }
        out << "------------------------------------------\n\n";

        file.close();
        return true;
    }
};

#endif //PROB_THEORY_HUB_HISTORYLOGGER_HPP
