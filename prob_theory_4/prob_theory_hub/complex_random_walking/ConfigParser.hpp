//
// Created by mvideo on 10/03/26.
//

#ifndef PROB_THEORY_HUB_CONFIGPARSER_HPP
#define PROB_THEORY_HUB_CONFIGPARSER_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "WalkEngine.hpp"
#include "../math/distribution.hpp"

class ConfigParser_4
{
public:
    struct ParseResult
    {
        WalkConfig config;
        i_distribution_model* model = nullptr;
        bool success = false;
        QString errorString;
    };

    static ParseResult parseJson (const QString& filename)
    {
        ParseResult result;
        QFile file(filename);

        if(!file.open(QIODevice::ReadOnly))
        {
            result.errorString = "Cannot open file of configuration";
            return result;
        }

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if(doc.isNull())
        {
            result.errorString = "Error format JSON";
            return result;
        }

        QJsonObject obj = doc.object();

        result.config.M = obj["M"].toInt(1000);
        result.config.K = obj["K"].toInt(500);
        result.config.p = obj["rho"].toDouble(1.0);
        result.config.n = obj["n"].toInt(4);

        QString distType = obj["distribution"].toString().toLower();

        size_t n = static_cast<size_t>(result.config.n);

        try
        {
            if (distType == "uniform")
            {
                result.model = new uniform_distribution_model(n);
            }
            else if (distType == "binomial")
            {
                double p = obj["p_dist"].toDouble(0.5);
                result.model = new binomial_distribution(p, n);
            }
            else if (distType == "triangular")
            {
                result.model = new triangular(n);
            }
            else if (distType == "geometric")
            {
                double p = obj["p_dist"].toDouble(0.5);
                result.model = new geometric_distribution(p, n);
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
                    result.model = new uniform_distribution_model(n);
                }
            }
            result.success = true;
        }
        catch(const std::exception& e)
        {
            result.errorString = QString("Error initializing model %1").arg(e.what());
            result.success = false;
        }
        return result;
    }
};

#endif //PROB_THEORY_HUB_CONFIGPARSER_HPP
