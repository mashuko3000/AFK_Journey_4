//
// Created by mvideo on 11/03/26.
//

#include "StairsWindow.hpp"
#include"ConfigLoader.hpp"

StairsWindow::StairsWindow(QWidget *parent) : QMainWindow(parent), m_simulator(nullptr)
{
    setMinimumSize(1000, 700);
    buildUI();
    applyStyle();
    updateControlsState(false);
}

void StairsWindow::buildUI()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    QFrame* headerFrame = new QFrame(this);
    headerFrame->setObjectName("headerContainer");
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);

    m_labelTitle = new QLabel("ProbTheoryHub", this);
    m_labelTitle->setObjectName("mainTitle");

    m_labelProbability = new QLabel("P: 0.0000", this);
    m_labelProbability->setObjectName("probabilityLabel");
    m_labelProbability->setAlignment(Qt::AlignCenter);

    QVBoxLayout* importBlock = new QVBoxLayout();
    m_btnImport = new QPushButton("Import JSON", this);
    m_btnImport->setObjectName("importButton");
    m_labelFileName = new QLabel("No file selected", this);
    m_labelFileName->setObjectName("fileNameLabel");
    importBlock->addWidget(m_btnImport);
    importBlock->addWidget(m_labelFileName);

    headerLayout->addWidget(m_labelTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(m_labelProbability);
    headerLayout->addStretch();
    headerLayout->addLayout(importBlock);

    mainLayout->addWidget(headerFrame);

    QHBoxLayout* contentLayout = new QHBoxLayout();

    QFrame* sideFrame = new QFrame(this);
    sideFrame->setObjectName("sidebarContainer");
    sideFrame->setFixedWidth(280);
    QVBoxLayout* sideLayout = new QVBoxLayout(sideFrame);

    QFormLayout* settingsForm = new QFormLayout();
    m_sbM = new QDoubleSpinBox(this);
    m_sbH = new QDoubleSpinBox(this);
    m_sbTau = new QDoubleSpinBox(this);
    m_sbN_range = new QSpinBox(this);
    m_sbN_trials = new QSpinBox(this);

    QList<QAbstractSpinBox*> spinners = {m_sbM, m_sbH, m_sbTau, m_sbN_range, m_sbN_trials};
    for(auto s : spinners) {
        s->setReadOnly(true);
        s->setButtonSymbols(QAbstractSpinBox::NoButtons);
        s->setObjectName("settingSpinner");
    }

    settingsForm->addRow("Max Length (M):", m_sbM);
    settingsForm->addRow("Step (h):", m_sbH);
    settingsForm->addRow("Scale (tau):", m_sbTau);
    settingsForm->addRow("Range (n):", m_sbN_range);
    settingsForm->addRow("Trials (N):", m_sbN_trials);

    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setObjectName("dividerLine");
    settingsForm->addRow(line);

    m_comboDistType = new QComboBox(this);
    m_comboDistType->setFocusPolicy(Qt::NoFocus);
    m_comboDistType->setEnabled(false);
    m_comboDistType->setStyleSheet("QComboBox::drop-down { border: none; }"
                                  "QComboBox::down-arrow { image: none; }"
                                  "QComboBox { padding-right: 0px; }");
    m_comboDistType->addItems({"Uniform", "Bernoulli", "Triangular"});
    m_comboDistType->setObjectName("distCombo");
    m_comboDistType->setEnabled(false);
    settingsForm->addRow("Law:", m_comboDistType);

    m_sbDistParam = new QDoubleSpinBox(this);
    m_sbDistParam->setObjectName("settingSpinner");
    m_sbDistParam->setReadOnly(true);
    m_sbDistParam->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_labelDistParam = new QLabel("Param:", this);
    m_labelDistParam->setObjectName("distParamLabel");

    settingsForm->addRow(m_labelDistParam, m_sbDistParam);

    m_btnRun = new QPushButton("RUN", this);
    m_btnRun->setObjectName("runButton");
    m_btnReset = new QPushButton("RESET", this);
    m_btnReset->setObjectName("resetButton");

    sideLayout->addLayout(settingsForm);
    sideLayout->addStretch();
    sideLayout->addWidget(m_btnRun);
    sideLayout->addWidget(m_btnReset);

    QFrame* plotFrame = new QFrame(this);
    plotFrame->setObjectName("plotContainer");
    QVBoxLayout* plotLayout = new QVBoxLayout(plotFrame);

    m_plot = new QCustomPlot(this);
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    QHBoxLayout* navLayout = new QHBoxLayout();
    m_btnPrev = new QPushButton("←", this);
    m_btnPrev->setObjectName("navBtn");
    m_btnNext = new QPushButton("→", this);
    m_btnNext->setObjectName("navBtn");
    m_comboSuccessTrials = new QComboBox(this);
    m_labelCurrentIdx = new QLabel("Trial: 0/0", this);

    navLayout->addWidget(m_btnPrev);
    navLayout->addWidget(m_labelCurrentIdx);
    navLayout->addWidget(m_comboSuccessTrials, 1);
    navLayout->addWidget(m_btnNext);

    plotLayout->addWidget(m_plot);
    plotLayout->addLayout(navLayout);

    contentLayout->addWidget(sideFrame);
    contentLayout->addWidget(plotFrame, 1);

    mainLayout->addLayout(contentLayout);

    connect(m_btnImport, &QPushButton::clicked, this, &StairsWindow::onImportConfig);
    connect(m_btnRun,&QPushButton::clicked, this, &StairsWindow::onRun);
    connect(m_btnReset,&QPushButton::clicked, this, &StairsWindow::onReset);
    connect(m_btnNext, &QPushButton::clicked, this, &StairsWindow::onNext);
    connect(m_btnPrev,&QPushButton::clicked, this, &StairsWindow::onPrev);
    connect(m_comboSuccessTrials, QOverload<int>::of(&QComboBox::activated), this, &StairsWindow::onSuccessSelect);
}

void StairsWindow::onImportConfig()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Staircase Config", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    try {
        ConfigLoader::load(fileName, m_currentStairsCfg, m_currentDistCfg);

        if (m_simulator) {
            delete m_simulator;
            m_simulator = nullptr;
        }

        m_simulator = new StaircaseSimulator(m_currentStairsCfg, m_currentDistCfg);

        m_sbM->setValue(m_currentStairsCfg.M);
        m_sbH->setValue(m_currentStairsCfg.h);
        m_sbTau->setValue(m_currentStairsCfg.tau);
        m_sbN_range->setValue(m_currentStairsCfg.n);
        m_sbN_trials->setValue(m_currentStairsCfg.N);

        m_comboDistType->setCurrentText(m_currentDistCfg.type);

        if (m_currentDistCfg.props.contains("p")) {
            m_labelDistParam->setText("Parm p:");
            m_sbDistParam->setValue(m_currentDistCfg.props["p"].toDouble());
            m_sbDistParam->show();
            m_labelDistParam->show();
        } else {
            m_sbDistParam->hide();
            m_labelDistParam->hide();
        }

        m_labelFileName->setText(QFileInfo(fileName).fileName());

        updateControlsState(false);

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", "Failed to load config: " + QString(e.what()));
    }
}
void StairsWindow::onRun()
{
    if (!m_simulator) return;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    m_simulator->run();

    QApplication::restoreOverrideCursor();

    simulationResult res = m_simulator->getStats();
    m_labelProbability->setText(QString("P: %1").arg(res.empiricalProbability, 0, 'f', 4));

    m_comboSuccessTrials->clear();
    std::vector<size_t> successIdxs = m_simulator->getSuccessfulIndices();

    for (size_t idx : successIdxs) {
        m_comboSuccessTrials->addItem(QString("Success №%1").arg(idx), QVariant::fromValue(idx));
    }

    updateControlsState(true);

    if (!successIdxs.empty()) {
        updatePlot(successIdxs[0]);
        m_labelCurrentIdx->setText(QString("Success: 1/%1").arg(successIdxs.size()));
    }
}
void StairsWindow::onReset()
{
    if (m_simulator) {
        m_simulator->reset();
    }

    m_plot->clearGraphs();
    m_plot->replot();

    m_labelProbability->setText("P: 0.0000");
    m_comboSuccessTrials->clear();
    m_labelCurrentIdx->setText("Trial: 0/0");

    updateControlsState(false);
}

void StairsWindow::onPrev()
{
    if (m_currentDisplayIdx > 0) {
        m_currentDisplayIdx--;
        updatePlot(m_currentDisplayIdx);
    }
}

void StairsWindow::onNext()
{
    if (m_currentDisplayIdx < (size_t)m_currentStairsCfg.N - 1) {
        m_currentDisplayIdx++;
        updatePlot(m_currentDisplayIdx);
    }
}

void StairsWindow::onSuccessSelect(int comboIdx)
{
    if (comboIdx < 0) return;

    size_t globalIdx = m_comboSuccessTrials->itemData(comboIdx).toULongLong();

    m_currentDisplayIdx = globalIdx;
    updatePlot(m_currentDisplayIdx);
}


void StairsWindow::updatePlot(size_t trialIndex)
{
    if (!m_simulator) return;

    std::vector<double> trial = m_simulator->getTrial(trialIndex);

    m_plot->clearGraphs();
    QCPGraph* graph = m_plot->addGraph();

    QVector<double> x, y;
    x.append(0);
    y.append(0);

    double h = m_currentStairsCfg.h;
    for (int i = 0; i < (int)trial.size(); ++i) {
        x.append((i + 1) * h);
        y.append(trial[i]);
    }

    graph->setData(x, y);
    graph->setLineStyle(QCPGraph::lsStepLeft);

    bool isSuccess = m_simulator->checkStrictlyIncreasing(trial);
    QPen pen(isSuccess ? QColor("#FF85A1") : QColor("#9CA3AF"), 3);
    graph->setPen(pen);

    m_plot->xAxis->setRange(0, m_currentStairsCfg.M + h);
    m_plot->yAxis->setRange(0, m_currentStairsCfg.M + 0.5);

    m_labelCurrentIdx->setText(QString("Iteration: %1 / %2 %3")
                                       .arg(trialIndex + 1)
                                       .arg(m_currentStairsCfg.N)
                                       .arg(isSuccess ? "⭐" : "❌"));

    m_plot->replot();
}
void StairsWindow::updateControlsState(bool calculated)
{
    bool hasSimulator = (m_simulator != nullptr);
    m_btnRun->setEnabled(hasSimulator && !calculated);
    m_btnReset->setEnabled(calculated || hasSimulator);
    bool canNavigate = calculated && (m_comboSuccessTrials->count() > 0);
    m_btnPrev->setEnabled(canNavigate);
    m_btnNext->setEnabled(canNavigate);
    m_comboSuccessTrials->setEnabled(canNavigate);
    m_btnRun->setProperty("active", !calculated && hasSimulator);
    m_labelProbability->setProperty("highlight", calculated);

    m_btnRun->style()->unpolish(m_btnRun);
    m_btnRun->style()->polish(m_btnRun);
    m_labelProbability->style()->unpolish(m_labelProbability);
    m_labelProbability->style()->polish(m_labelProbability);
}
void StairsWindow::applyStyle()
{
        QString styleSheet = R"(
        QMainWindow {
            background-color: #F9FAFB;
        }

        #headerContainer, #sidebarContainer, #plotContainer {
            background-color: #FFFFFF;
            border-radius: 30px;
            border: 2px solid #E5E7EB;
        }

        #mainTitle {
            color: #4B5563;
            font-family: 'Segoe UI Rounded', 'Arial Rounded MT Bold', sans-serif;
            font-size: 20px;
            font-weight: bold;
            margin-left: 10px;
        }

        #probabilityLabel {
            background-color: #FFD1DC;
            color: #4B5563;
            border: 2px dashed #6B7280;
            border-radius: 20px;
            padding: 8px 20px;
            font-size: 26px;
            font-weight: bold;
        }

        #settingSpinner {
            background-color: #F3F4F6;
            border: 2px solid #E5E7EB;
            border-radius: 12px;
            padding: 5px;
            color: #4B5563;
            font-weight: bold;
        }

        QPushButton {
            background-color: #E5E7EB;
            color: #4B5563;
            border: 2px dashed #9CA3AF;
            border-radius: 20px;
            padding: 10px 20px;
            font-weight: bold;
            font-size: 13px;
        }

        #dividerLine {
            border: none;
            border-top: 2px dashed #E5E7EB;
            max-height: 2px;
            margin: 10px 0;
        }
        #distParamLabel {
            color: #9CA3AF;
            font-style: italic;
        }

        QPushButton:hover {
            background-color: #FFD1DC;
            border: 2px dashed #4B5563;
        }

        #runButton {
            background-color: #FFD1DC;
            border: 2px dashed #4B5563;
            font-size: 15px;
        }

        #runButton:disabled {
            background-color: #F3F4F6;
            border: 2px dashed #D1D5DB;
            color: #9CA3AF;
        }

        #resetButton {
            background-color: #FFFFFF;
            border: 2px dashed #E5E7EB;
        }
        #resetButton:hover {
            background-color: #FF85A1;
            color: white;
        }
        QComboBox {
            background-color: #FFFFFF;
            border: 2px dashed #D1D5DB;
            border-radius: 15px;
            padding: 5px 15px;
            color: #4B5563;
        }

        #fileNameLabel { color: #9CA3AF; font-size: 11px; }

        #navBtn { font-size: 20px; }
    )";

        this->setStyleSheet(styleSheet);

        m_plot->setBackground(QBrush(Qt::white));
        QPen gridPen(QColor(229, 231, 235));
        gridPen.setStyle(Qt::DashLine);
        m_plot->xAxis->grid()->setPen(gridPen);
        m_plot->yAxis->grid()->setPen(gridPen);

        m_plot->xAxis->setBasePen(QPen(QColor(156, 163, 175), 2));
        m_plot->yAxis->setBasePen(QPen(QColor(156, 163, 175), 2));
        m_plot->xAxis->setTickLabelColor(QColor(75, 85, 99));
        m_plot->yAxis->setTickLabelColor(QColor(75, 85, 99));

        m_plot->replot();
}