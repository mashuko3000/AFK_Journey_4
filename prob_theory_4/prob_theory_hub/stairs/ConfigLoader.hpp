//
// Created by mvideo on 11/03/26.
//

#ifndef PROB_THEORY_HUB_CONFIGLOADER_HPP
#define PROB_THEORY_HUB_CONFIGLOADER_HPP

#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <stdexcept>
#include "StaircaseSimulator.hpp"

class ConfigLoader
{
public:
    static bool load(const QString& filePath, stairsConfig& stCfg, DistributionParams& distCfg)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
        {
            throw std::runtime_error("Cannot open config file: " + filePath.toStdString());
        }

        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);

        if (doc.isNull() || !doc.isObject())
        {
            throw std::runtime_error("Invalid JSON format");
        }

        QJsonObject root = doc.object();

        parseStairsConfig(root, stCfg);

        parseDistributionConfig(root, distCfg);

        return true;
    }

private:
    static void parseStairsConfig(const QJsonObject& obj, stairsConfig& cfg)
    {
        const QStringList required = {"M", "h", "tau", "n", "N"};
        for (const QString& key : required)
        {
            if (!obj.contains(key))
            {
                throw std::invalid_argument("Missing required field: " + key.toStdString());
            }
        }

        cfg.M = obj["M"].toDouble();
        cfg.h = obj["h"].toDouble();
        cfg.tau = obj["tau"].toDouble();
        cfg.n = obj["n"].toInt();
        cfg.N = obj["N"].toInt();
    }

    static void parseDistributionConfig(const QJsonObject& obj, DistributionParams& cfg)
    {
        if (!obj.contains("distribution") || !obj["distribution"].isObject())
        {
            throw std::invalid_argument("Missing 'distribution' object");
        }

        QJsonObject distObj = obj["distribution"].toObject();
        if (!distObj.contains("type"))
        {
            throw std::invalid_argument("Distribution 'type' not specified");
        }

        cfg.type = distObj["type"].toString();

        if (distObj.contains("params") && distObj["params"].isObject())
        {
            cfg.props = distObj["params"].toObject();
        }
    }
};

#endif //PROB_THEORY_HUB_CONFIGLOADER_HPP
