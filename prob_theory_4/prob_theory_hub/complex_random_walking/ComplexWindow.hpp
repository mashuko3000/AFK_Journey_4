//
// Created by mvideo on 10/03/26.
//

#ifndef PROB_THEORY_HUB_COMPLEXWINDOW_HPP
#define PROB_THEORY_HUB_COMPLEXWINDOW_HPP

#include <QMainWindow>
#include <complex>
#include <vector>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QGraphicsBlurEffect>

#include "WalkEngine.hpp"
#include "qcustomplot.h"

class ComplexWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ComplexWindow(QWidget* parent = nullptr);
    ~ComplexWindow();

private slots:
    void onImportJson();
    void onRunAll();
    void onStep();
    void onReset();
    void onCenterCamera();

private:
    void buildUI();
    void setupPlot();
    void applyGlassStyle();

    void drawTrajectory(const TrajectoryResult& res);
    void updateLiveStatus(const std::complex<double>& pos);

    void updateConfigInfo(const QString& path);

private:
    WalkEngine* m_engine = nullptr;
    WalkConfig* m_config;

    QCustomPlot* m_plot;

    QFrame* m_cardControl;
    QFrame* m_cardConfig;
    QFrame* m_cardStatus;

    QLabel* m_lblProjectTitle;
    QLabel* m_lblLivePos;
    QLabel* m_lblProbability;
    QLabel* m_lblConfigInfo;

    QPoint m_dragPos;
    bool m_dragging = false;
    QFrame* m_currentDragged = nullptr;

    QPushButton *btnRun, *btnStep, *btnReset, *btnImport, *btnCenter;

    bool m_isStepByStep = false;
};


#endif //PROB_THEORY_HUB_COMPLEXWINDOW_HPP
