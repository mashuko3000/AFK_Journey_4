//
// Created by mvideo on 08/03/26.
//

#ifndef PROB_THEORY_HUB_CONFIG_HPP
#define PROB_THEORY_HUB_CONFIG_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "RandomWalking.hpp"
#include "../math/distribution.hpp"

class ConfigParser
{
public:
    struct ParseResult
    {
        simulationConfig config;
        i_distribution_model* model = nullptr;
        bool success = false;
        QString errorString;
    };

    static ParseResult parseJson(const QString& filePath)
    {
        ParseResult result;
        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly))
        {
            result.errorString = "Could not open file";
            return result;
        }

        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull())
        {
            result.errorString = "Invalid JSON format";
            return result;
        }

        QJsonObject obj = doc.object();

        result.config.initialY = obj["initialY"].toDouble(0.0);
        result.config.stepH = obj["h"].toDouble(1.0);
        result.config.stepsPerTrial = obj["n"].toInt(100);
        result.config.trials = obj["M"].toInt(1000);
        result.config.targetCrossingL = obj["l"].toInt(1);
        result.config.logLimitK = obj["K"].toInt(5);

        QJsonArray sArray = obj["s_values"].toArray();
        QVector<double> sValues;
        for (auto val : sArray)
        {
            sValues.append(val.toDouble());
        }
        result.config.s_values = sValues;

        QString distType = obj["distribution"].toString().toLower();
        size_t m = static_cast<size_t>(sValues.size());

        if (distType == "uniform")
        {
            result.model = new uniform_distribution_model(m);
        }
        else if (distType == "binomial")
        {
            double p = obj["p"].toDouble(0.5);
            result.model = new binomial_distribution(p, m);
        }
        else if (distType == "triangular")
        {
            result.model = new triangular(m);
        }
        else if (distType == "geometric")
        {
            double p = obj["p"].toDouble(0.5);
            result.model = new geometric_distribution(p, m);
        }
        else
        {
            QJsonArray weightsArray = obj["weights"].toArray();
            std::vector<double> weights;
            for(auto w : weightsArray) weights.push_back(w.toDouble());
            if (!weights.empty())
            {
                result.model = new discrete_distribution_model(weights);
            }
            else
            {
                result.model = new uniform_distribution_model(m);
            }
        }
        result.success = true;
        return result;
    }
};

#endif //PROB_THEORY_HUB_CONFIG_HPP
