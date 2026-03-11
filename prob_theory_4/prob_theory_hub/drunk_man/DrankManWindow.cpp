//
// Created by mvideo on 07/03/26.
//

#include "DrankManWindow.hpp"
#include "../external/qcustomplot.h"

QMap<QString, ControlSchema> DrankManWindow::createSchema()
{
    QMap<QString, ControlSchema> s;

    s["p"] = {"p", "Probability (p)", ControlType::DoubleSpin, 0.0, 1.0, 0.5, 2};
    s["B"] = {"B", "Cafe (B)", ControlType::DoubleSpin, 1.0, 1000.0, 10.0, 1};
    s["max_steps"] = {"max_steps", "Max count of steps (K)", ControlType::IntSpin, 10, 1000000, 5000};
    s["batch_size"] = {"batch_size", "Multiple steps ", ControlType::IntSpin, 1, 10000, 100};

    return s;
}

DrankManWindow::DrankManWindow(QWidget* parent)
        : UniversalBaseWindow("DRUNK MAN AND ABYSS", createSchema(), parent)
{
    m_engine = new DrunkardEngine(10, 0, 0.5);
    onResetStep();
}

DrankManWindow::~DrankManWindow()
{
    delete m_engine;
}

void DrankManWindow::onSingleStep()
{
    syncEngine();
    TrialResult res = m_engine->runOneTrial(getInt("max_steps"));
    m_allTrials.append(res);

    updateUI();
    setPaginationVisible(true);
    drawTrial(m_allTrials.size() - 1);
}

void DrankManWindow::onPrevPage()
{
    if (m_currentPage > 0)
    {
        m_currentPage--;
        drawTrial(m_currentPage);
        updatePagination(m_currentPage + 1, m_allTrials.size());
    }
}

void DrankManWindow::onNextPage()
{
    if (m_currentPage + 1 < m_allTrials.size())
    {
        m_currentPage++;
        drawTrial(m_currentPage);
        updatePagination(m_currentPage + 1, m_allTrials.size());
    }
}

void DrankManWindow::drawTrial(int index)
{
    if (index < 0 || index >= m_allTrials.size())
    {
        log("ERROR : index trajectory out of range");
        return;
    }

    const TrialResult& res = m_allTrials[index];

    clearPlot();

    if (res.trajectory.isEmpty()) {
        log("Trajectory is empty - ignore");
        return;
    }

    QVector<double> x, y;
    x.reserve(res.trajectory.size());
    y.reserve(res.trajectory.size());

    for (int i = 0; i < res.trajectory.size(); ++i) {
        x << static_cast<double>(i);
        y << res.trajectory[i];
    }

    QColor color = res.fell ? Qt::red : Qt::green;
    QString status = res.fell ? "fell into the arms of god" : (res.returned ? "came back to fill up with beer" : "go away to sleep");

    drawLine(x, y,QString("Trajectory #%1 (%2)").arg(index + 1).arg(status), color, 2);

    if (!x.isEmpty())
    {
        double x_min = *std::min_element(x.begin(), x.end());
        double x_max = *std::max_element(x.begin(), x.end());
        double y_min = *std::min_element(y.begin(), y.end());
        double y_max = *std::max_element(y.begin(), y.end());

        double x_padding = (x_max - x_min) * 0.1;
        double y_padding = (y_max - y_min) * 0.2;

        m_plot->xAxis->setRange(x_min - x_padding, x_max + x_padding);
        m_plot->yAxis->setRange(y_min - y_padding, y_max + y_padding);
    }

    m_plot->replot();

    log(QString("Trajectory showed #%1: %2 (steps: %3)")
                .arg(index + 1)
                .arg(status)
                .arg(res.trajectory.size() - 1));
}

void DrankManWindow::onBatchStep()
{
    syncEngine();
    int m = getInt("batch_size");
    int k = getInt("max_steps");

    for(int i = 0; i < m; ++i)
    {
        m_allTrials.append(m_engine->runOneTrial(k));
    }

    updateUI();
    setPaginationVisible(true);
}

void DrankManWindow::onPageChanged(int index)
{
    if (index < 0 || index >= m_allTrials.size())
    {
        m_plot->clearGraphs();
        m_plot->replot();
        return;
    }

    const auto& trial = m_allTrials[index];

    m_plot->clearGraphs();
    auto graph = m_plot->addGraph();

    QVector<double> x, y;
    for(int i = 0; i < trial.trajectory.size(); ++i)
    {
        x << i;
        y << trial.trajectory[i];
    }
    graph->setData(x, y);

    m_plot->rescaleAxes();
    m_plot->replot();

    log(QString("Page %1: %2 steps").arg(index + 1).arg(x.size()));
}

void DrankManWindow::syncEngine()
{
    if (!m_engine) return;
    m_engine->setParams(getDouble("B"), 0.0, getDouble("p"));
}

void DrankManWindow::updateUI()
{
    int total = m_allTrials.size();

    if (m_pageLabel) {
        m_pageLabel->setText(QString("Result %1 из %2")
                                     .arg(m_currentPage + 1)
                                     .arg(total));
    }

    if (m_btnPrev) m_btnPrev->setEnabled(m_currentPage > 0);
    if (m_btnNext) m_btnNext->setEnabled(m_currentPage + 1 < total);

    if (m_engine)
    {
        setStat("P(fall)",   m_engine->getFallProbability(), "");
        setStat("P(return)", m_engine->getReturnProbability(), "");
    }
}

void DrankManWindow::onResetStep()
{
    if (m_pageSpin) m_pageSpin->blockSignals(true);

    m_engine->resetStatistics();
    m_allTrials.clear();
    m_currentPage = 0;

    if (m_pageSpin)
    {
        m_pageSpin->setRange(0, 0);
        m_pageSpin->setValue(0);
    }

    if (m_pageLabel) m_pageLabel->setText("из 0");

    if (m_pageSpin) m_pageSpin->blockSignals(false);

    if (m_plot) {
        m_plot->clearGraphs();
        m_plot->xAxis->setRange(0, 10);
        m_plot->yAxis->setRange(0, 10);
        m_plot->replot();
    }

    updateUI();
    log("System restored.");
}

void DrankManWindow::runCalculation(const QJsonObject& config)
{
    if (!m_plot || !m_engine) return;

    onResetStep();
    log("Launching global simulation: Probability f(B)...");

    double p = config["p"].toDouble();
    double maxB = config["B"].toDouble();
    int M = config["batch_size"].toInt();
    int K = config["max_steps"].toInt();

    QVector<double> xB, yFall, yReturn;

    for (double b = 1.0; b <= maxB; b += 1.0)
    {
        m_engine->setParams(b, 0.0, p);
        m_engine->resetStatistics();

        for (int i = 0; i < M; ++i) {
            m_engine->runOneTrial(K);
        }

        xB << b;
        yFall << m_engine->getFallProbability();
        yReturn << m_engine->getReturnProbability();
    }

    m_plot->clearGraphs();

    auto graphFall = m_plot->addGraph();
    graphFall->setData(xB, yFall);
    graphFall->setPen(QPen(QColor(255, 105, 180), 2));
    graphFall->setName("P (fell)");

    auto graphReturn = m_plot->addGraph();
    graphReturn->setData(xB, yReturn);
    graphReturn->setPen(QPen(Qt::gray, 2));
    graphReturn->setName("P (return)");

    m_plot->xAxis->setLabel("Distance to the abyss (B)");
    m_plot->yAxis->setLabel("Probability");
    m_plot->yAxis->setRange(0, 1.1);
    m_plot->legend->setVisible(true);

    m_plot->rescaleAxes();
    m_plot->replot();

    if (m_engine) {
        setStat("P(fall)",   m_engine->getFallProbability(), "");
        setStat("P(return)", m_engine->getReturnProbability(), "");
    }
    setPaginationVisible(false);
    log(QString("Calculation is over. Checked B points: %1. Tests per point: %2").arg(xB.size()).arg(M));
}