//
// Created by mvideo on 06/03/26.
//

#ifndef PROB_THEORY_HUB_BASE_ENUMS_HPP
#define PROB_THEORY_HUB_BASE_ENUMS_HPP

#include <QMainWindow>
#include <QMap>
#include <QString>
#include <QJsonObject>
#include <QVector>
#include <QVariant>
#include <complex>

enum class ControlType
{
    DoubleSpin,
    IntSpin,
    BigIntSpin,
    ComboBox,
    Distribution,
    ComplexPlotMode
};

enum class DistributionLaw
{
    Uniform,
    Binomial,
    Geometric,
    DiscreteTriangular
};

struct DistParameters
{
    DistributionLaw law = DistributionLaw::Uniform;
    double p = 0.5;
    int trials = 10;
    double a = 0.0, b = 1.0;
    double c = 0.5;
    int maxValue = 100;
};

struct ControlSchema
{
    QString key;
    QString label;
    ControlType type = ControlType::DoubleSpin;

    double min = 0.0;
    double max = 1e9;
    QVariant defaultValue = 0.0;

    int decimals = 4;
    bool useScientificNotation = false;

    QStringList options;
    DistParameters defaultDist;
};
#endif //PROB_THEORY_HUB_BASE_ENUMS_HPP
