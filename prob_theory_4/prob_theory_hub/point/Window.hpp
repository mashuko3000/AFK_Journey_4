//
// Created by mvideo on 08/03/26.
//

#ifndef PROB_THEORY_HUB_WINDOW_HPP
#define PROB_THEORY_HUB_WINDOW_HPP

#include <QMainWindow>
#include <QVector>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QGridLayout>
#include "RandomWalking.hpp"

#include "qcustomplot.h"

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget* parent = nullptr);
    ~Window();

private slots:

    void onImportJsonClicked();
    void onSelectExportFileClicked();
    void onRunSingleClicked();
    void onRunAllClicked();
    void onResetClicked();

private:
    void buildUI();
    void applyBentoStyle();
    void updateButtonsState();

    void header();
    void leftPanelSet();
    void rightPanelSet();

    QWidget* createCard(QWidget* content, const QString& title);

private:
    simulationConfig m_config;
    simulationSummary m_summary;
    RandomWalking* m_worker = nullptr;
    QString m_exportFilePath;
    bool m_jsonLoaded = false;

    void plotTrajectory(const trialResult& res, bool clearOld);
    void setupZeroLine();
    void updateDisplayParams();

private:
    QDoubleSpinBox *sbY, *sbH;
    QSpinBox *sbN, *sbL, *sbK;
    QLabel *lblDistType, *lblExportPath;

    QPushButton *btnRunSingle, *btnRunAll, *btnSelectExport, *btnImport, *btnReset;
    QCustomPlot *m_plot;
    QTextEdit *teLogs;
    QLabel *lblProb;

private:
    QWidget* centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *contentLayout;

    QVBoxLayout *leftPanel;
    QVBoxLayout *rightPanel;
};


#endif //PROB_THEORY_HUB_WINDOW_HPP
