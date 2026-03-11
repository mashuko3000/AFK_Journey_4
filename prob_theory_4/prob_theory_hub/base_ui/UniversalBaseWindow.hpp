//
// Created by mvideo on 06/03/26.
//

#ifndef PROB_THEORY_HUB_UNIVERSALBASEWINDOW_HPP
#define PROB_THEORY_HUB_UNIVERSALBASEWINDOW_HPP

#include<QMainWindow>
#include <QMap>
#include <QString>
#include <QJsonObject>
#include <QVector>
#include <QVariant>
#include <complex>

#include "base_enums.hpp"

class QVBoxLayout;
class QFormLayout;
class QDoubleSpinBox;
class QSpinBox;
class QComboBox;
class QLabel;
class QTextEdit;
class QCustomPlot;
class QPushButton;
class QStackedWidget;
class QGroupBox;

class UniversalBaseWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit UniversalBaseWindow(const QString& title,
                                 const QMap<QString, ControlSchema>& schema,
                                 QWidget *parent = nullptr);
    virtual ~UniversalBaseWindow();

protected:
    virtual void runCalculation(const QJsonObject& config) = 0;
    virtual void onPageChanged(int index) { Q_UNUSED(index); }

    double getDouble(const QString& key) const;
    qint64 getBigInt(const QString& key) const;
    int getInt(const QString& key) const;
    QString getString(const QString& key) const;
    DistributionLaw getDistributionLaw(const QString& key) const;
    DistParameters getDistParams(const QString& key) const;

    void clearPlot();
    void drawLine(const QVector<double>& x, const QVector<double>& y,
                  const QString& name, QColor color = Qt::blue, int width = 2);
    void drawBars(const QVector<double>& x, const QVector<double>& y,
                  const QString& name, QColor color = Qt::blue);
    void drawComplexTrajectory(const QVector<std::complex<double>>& path,
                               const QString& name, QColor color = Qt::darkGreen);

    void setStat(const QString& key, const QVariant& value, const QString& unit);
    void log(const QString& message);

    void clearResults();
    void addTrajectory(const QVector<QPointF>& traj, const QString& description = {});
    void addComplexTrajectory(const QVector<std::complex<double>>& path, const QString& desc = {});
    void addStepFunction(const QVector<double>& heights, const QString& desc = {});
    void setResultCount(int count);
    void showResult(int index);

    void setPaginationVisible(bool visible);
    void updatePagination(int current, int total);

protected:
    virtual void onResetStep();
    virtual void onStartTriggered();
    virtual void onSingleStep();
    virtual void onBatchStep();

protected slots:
    virtual void onPrevPage();
    virtual void onNextPage();
    void onExportConfig();
    void onImportConfig();
    void onDistributionChanged(const QString& key, int index);
    void onPageButtonClicked();

protected:

    void applyConfig(const QJsonObject& config);

    int getTotalResultsCount() const;
    void buildUI(const QString& title, const QMap<QString, ControlSchema>& schema);
    void setupConnections();
    QWidget* createInputWidget(const ControlSchema& sch, const QString& key);
    QJsonObject collectCurrentConfig() const;
    QWidget* createDistributionPanel(const QString& key, const DistParameters& defaults);

    QMap<QString, QWidget*> m_inputs;
    QMap<QString, QGroupBox*> m_distGroups;
    QMap<QString, QLabel*> m_stats;

    QCustomPlot* m_plot = nullptr;
    QTextEdit* m_log = nullptr;
    QVBoxLayout* m_leftLayout = nullptr;
    QLabel* m_pageLabel = nullptr;
    QPushButton* m_btnStart = nullptr;
    QPushButton* m_btnStep = nullptr;
    QPushButton* m_btnBatch = nullptr;
    QPushButton* m_btnReset = nullptr;
    QPushButton* m_btnPrev = nullptr;
    QPushButton* m_btnNext = nullptr;
    QSpinBox* m_pageSpin = nullptr;
    QWidget* m_paginationContainer = nullptr;
    QFormLayout* m_statsLayout = nullptr;

protected:
    QVector<QVector<QPointF>>m_trajectories;
    QVector<QVector<QPointF>>m_complexPaths;
    QVector<QVector<double>>m_stepFunctions;
    QVector<QString>m_resultDescriptions;

    int m_currentPage = 0;
};


#endif //PROB_THEORY_HUB_UNIVERSALBASEWINDOW_HPP
