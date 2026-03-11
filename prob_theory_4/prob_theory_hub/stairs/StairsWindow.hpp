//
// Created by mvideo on 11/03/26.
//

#ifndef PROB_THEORY_HUB_STAIRSWINDOW_HPP
#define PROB_THEORY_HUB_STAIRSWINDOW_HPP

#include <QMainWindow>
#include <memory>
#include "StaircaseSimulator.hpp"
#include "qcustomplot.h"

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

class StairsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StairsWindow(QWidget *parent = nullptr);
    ~StairsWindow()
    {
        delete m_simulator;
    }

private slots:
    void onImportConfig();
    void onRun();
    void onReset();
    void onPrev();
    void onNext();
    void onSuccessSelect(int idx);

private:
    void buildUI();
    void updatePlot(size_t trialIndex);
    void updateControlsState(bool calculated);
    void applyStyle();

private:
    StaircaseSimulator* m_simulator;
    stairsConfig m_currentStairsCfg;
    DistributionParams m_currentDistCfg;

    size_t m_currentDisplayIdx = 0;

    QLabel* m_labelTitle;
    QLabel* m_labelProbability;
    QPushButton* m_btnImport;
    QLabel* m_labelFileName;
    QLabel* m_labelDistParam;

    QDoubleSpinBox* m_sbM;
    QDoubleSpinBox* m_sbH;
    QDoubleSpinBox* m_sbTau;
    QDoubleSpinBox* m_sbDistParam;
    QSpinBox* m_sbN_range;
    QSpinBox* m_sbN_trials;
    QComboBox* m_comboDistType;

    QPushButton* m_btnRun;
    QPushButton* m_btnReset;

    QCustomPlot* m_plot;

    QPushButton* m_btnPrev;
    QPushButton* m_btnNext;
    QComboBox* m_comboSuccessTrials;
    QLabel* m_labelCurrentIdx;
};


#endif //PROB_THEORY_HUB_STAIRSWINDOW_HPP
