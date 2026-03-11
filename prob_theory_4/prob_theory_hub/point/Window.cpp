//
// Created by mvideo on 08/03/26.
//

#include "Window.hpp"
#include "Config.hpp"
#include <QErrorMessage>
#include <QWidget>
#include "HistoryLogger.hpp"
#include "../external/qcustomplot.h"
#include <QRandomGenerator>


Window::Window(QWidget* parent) : QMainWindow(parent)
{
    buildUI();
    applyBentoStyle();
    updateButtonsState();
    resize(1000, 700);

    //qsrand(QTime::currentTime().msec());
}
Window::~Window()
{
    if (m_worker) delete m_worker;
}

void Window::onImportJsonClicked()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                "Open config : ",
                                                "",
                                                "JSON Files (*.json)");
    if (path.isEmpty()) return;

    ConfigParser::ParseResult result = ConfigParser::parseJson(path);

    if(!result.success)
    {
        QMessageBox::critical(
                this,
                tr("Parsing Error"),
                result.errorString
                );
        return;
    }

    m_config = result.config;

    if (m_worker) delete m_worker;
    m_worker = new RandomWalking(m_config, m_summary, result.model);

    sbY->setValue(m_config.initialY);
    sbH->setValue(m_config.stepH);
    sbN->setValue(m_config.stepsPerTrial);
    sbL->setValue(m_config.targetCrossingL);
    sbK->setValue(m_config.logLimitK);

    m_jsonLoaded = true;
    updateDisplayParams();
    updateButtonsState();

    teLogs->append("[Config loaded]");
}
void Window::onSelectExportFileClicked()
{
    m_exportFilePath = QFileDialog::getSaveFileName(this,
                                                    "Select the file for the logs",
                                                    "",
                                                    "Text Files (*.txt);;Log Files (*.log)");
    if(!m_exportFilePath.isEmpty())
    {
        lblExportPath->setText("File : " + m_exportFilePath.section('/', -1));
        updateButtonsState();
    }
}

void Window::updateButtonsState()
{
    bool ready = m_jsonLoaded && !m_exportFilePath.isEmpty();
    btnRunSingle->setEnabled(ready);
    btnRunAll->setEnabled(ready);
}

void Window::updateDisplayParams()
{
    sbY->setValue(m_config.initialY);
    sbH->setValue(m_config.stepH);
    sbN->setValue(m_config.stepsPerTrial);
    sbL->setValue(m_config.targetCrossingL);
    sbK->setValue(m_config.logLimitK);
}

void Window::onRunSingleClicked()
{
    if(!m_worker) return;

    trialResult res = m_worker->runSingle();
    plotTrajectory(res, false);
    m_summary = m_worker->get_summary();

    lblProb->setText(QString("Probability P : %1")
                             .arg(m_summary.empiricalProbability));

    teLogs -> append(QString("[%1]  Enumeration : %2 | %3")
                .arg(res.id)
                .arg(res.crossingCount)
                .arg(res.isMatch ? "SUCCESS" : "FAIL"));
    HistoryLogger::saveHistoryToFile(m_exportFilePath, m_summary);
}
void Window::onRunAllClicked()
{
    if(!m_worker) return;

    setEnabled(false);
    QApplication::processEvents();

    m_worker->runAll();
    m_summary = m_worker->get_summary();

    m_plot->clearGraphs();
    setupZeroLine();

    for(const auto& trial : m_summary.history)
    {
        plotTrajectory(trial, false);
    }

    lblProb->setText(QString("Probability P: %1   (n = %2, M = %3)")
                             .arg(m_summary.empiricalProbability, 0, 'f', 4)
                             .arg(m_config.stepsPerTrial)
                             .arg(m_config.trials));

    teLogs->append(QString("[===Finished %1 trials, P = %2===]")
                           .arg(m_summary.totalSimulation)
                           .arg(m_summary.empiricalProbability, 0, 'f', 4));

    if (!m_exportFilePath.isEmpty())
    {
        HistoryLogger::saveHistoryToFile(m_exportFilePath, m_summary);
    }

    setEnabled(true);
}

void Window::onResetClicked()
{
    if(m_worker) m_worker->reset();
    m_plot->clearGraphs();
    m_plot->addGraph();
    m_plot->replot();
    teLogs->clear();

    if(!m_exportFilePath.isEmpty())
    {
        QFile file(m_exportFilePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        {
            file.close();
            teLogs->append("Log file was cleared: " + m_exportFilePath.section('/', -1));
        }
        else
        {
            teLogs->append("Cant clean log file: " + file.errorString());
        }
    }

    lblProb->setText("Probability p : 0.000");
}
void Window::buildUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel* header = new QLabel("Probability Theory Hub: Random Walking (Task 2)");
    header->setObjectName("headerLabel");
    header->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(header);

    contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);

    leftPanel = new QVBoxLayout();
    leftPanel->setSpacing(15);

    QVBoxLayout* configLayout = new QVBoxLayout();
    configLayout->setSpacing(10);

    btnImport = new QPushButton("1. Load JSON Config");
    btnSelectExport = new QPushButton("2. Select Export File");
    lblExportPath = new QLabel("No file selected");
    lblExportPath->setWordWrap(true);
    lblExportPath->setStyleSheet("color: #666; font-style: italic;");

    configLayout->addWidget(btnImport);
    configLayout->addWidget(btnSelectExport);
    configLayout->addWidget(lblExportPath);

    QWidget* configContent = new QWidget();
    configContent->setLayout(configLayout);

    leftPanel->addWidget(createCard(configContent, "Configuration & Logs"));

    QGridLayout* paramsLayout = new QGridLayout();
    paramsLayout->setSpacing(8);

    paramsLayout->setContentsMargins(10, 10, 10, 10);

    sbY = new QDoubleSpinBox(); sbY->setReadOnly(true); sbY->setDecimals(2);
    sbH = new QDoubleSpinBox(); sbH->setReadOnly(true); sbH->setDecimals(2);
    sbN = new QSpinBox();       sbN->setReadOnly(true);
    sbL = new QSpinBox();       sbL->setReadOnly(true);
    sbK = new QSpinBox();       sbK->setReadOnly(true);

    paramsLayout->addWidget(new QLabel("Initial Y:"),     0, 0); paramsLayout->addWidget(sbY, 0, 1);
    paramsLayout->addWidget(new QLabel("Step h:"),        1, 0); paramsLayout->addWidget(sbH, 1, 1);
    paramsLayout->addWidget(new QLabel("Steps (n):"),    2, 0); paramsLayout->addWidget(sbN, 2, 1);
    paramsLayout->addWidget(new QLabel("Target L:"),      3, 0); paramsLayout->addWidget(sbL, 3, 1);
    paramsLayout->addWidget(new QLabel("Log Limit K:"),   4, 0); paramsLayout->addWidget(sbK, 4, 1);

    QWidget* paramsWidget = new QWidget();
    paramsWidget->setLayout(paramsLayout);

    leftPanel->addWidget(createCard(paramsWidget, "Model Parameters"));

    QVBoxLayout* actionsLayout = new QVBoxLayout();
    actionsLayout->setSpacing(10);
    actionsLayout->setContentsMargins(10, 10, 10, 10);



    btnRunSingle = new QPushButton("Run Single Trial");
    btnRunAll    = new QPushButton("Run All Simulations");
    btnReset     = new QPushButton("Reset Simulation");

    btnRunSingle->setEnabled(false);
    btnRunAll->setEnabled(false);

    actionsLayout->addWidget(btnRunSingle);
    actionsLayout->addWidget(btnRunAll);
    actionsLayout->addWidget(btnReset);

    QWidget* actionsWidget = new QWidget();
    actionsWidget->setLayout(actionsLayout);

    leftPanel->addWidget(createCard(actionsWidget, "Execution"));

    leftPanel->addStretch();

    rightPanel = new QVBoxLayout();

    m_plot = new QCustomPlot();
    m_plot->addGraph();
    m_plot->xAxis->setLabel("x (Steps × h)");
    m_plot->yAxis->setLabel("y (Coordinate)");
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    m_plot->setBackground(QColor("#FFF8FA"));
    m_plot->xAxis->setBasePen(QPen(QColor("#FF85A2"), 1));
    m_plot->yAxis->setBasePen(QPen(QColor("#FF85A2"), 1));

    lblProb = new QLabel("Empirical Probability P: 0.000");
    lblProb->setObjectName("probLabel");
    lblProb->setObjectName("probLabel");
    lblProb->setAlignment(Qt::AlignCenter);

    teLogs = new QTextEdit();
    teLogs->setObjectName("teLogs");
    teLogs->setReadOnly(true);
    teLogs->setPlaceholderText("Logs of the last K operations will appear here...");
    teLogs->setMinimumHeight(140);
    teLogs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    rightPanel->addWidget(createCard(m_plot, "Trajectory Visualization"), 6);
    rightPanel->addWidget(createCard(lblProb, "Result Analysis"), 1);
    rightPanel->addWidget(createCard(teLogs, "Simulation Logs"), 3);


    contentLayout->addLayout(leftPanel, 20);
    contentLayout->addLayout(rightPanel, 80);

    mainLayout->addLayout(contentLayout);

    connect(btnImport, &QPushButton::clicked, this, &Window::onImportJsonClicked);
    connect(btnSelectExport,&QPushButton::clicked, this, &Window::onSelectExportFileClicked);
    connect(btnRunSingle,&QPushButton::clicked, this, &Window::onRunSingleClicked);
    connect(btnRunAll, &QPushButton::clicked, this, &Window::onRunAllClicked);
    connect(btnReset,&QPushButton::clicked, this, &Window::onResetClicked);
}


QWidget* Window::createCard(QWidget* content, const QString& title)
{
    QWidget* card = new QWidget();
    card->setObjectName("bentoCard");

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(255, 105, 180, 60));
    shadow->setOffset(0, 4);
    card->setGraphicsEffect(shadow);

    if (!title.isEmpty()) {
        QLabel* titleLabel = new QLabel(title);
        titleLabel->setObjectName("cardTitle");
        titleLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        layout->addWidget(titleLabel);
    }

    if (content) {
        layout->addWidget(content);
        layout->setStretchFactor(content, 1);
    }

    return card;
}

void Window::setupZeroLine()
{
    QCPGraph* zeroLine = m_plot->addGraph();
    zeroLine->setPen(QPen(Qt::red, 1.2, Qt::DashLine));
    zeroLine->setData(QVector<double>{-1, 100}, QVector<double>{0, 0});
    zeroLine->setSelectable(QCP::stNone);
    zeroLine->setName("y = 0");
}

void Window::plotTrajectory(const trialResult& res, bool clearOld)
{
    if (clearOld) {
        m_plot->clearGraphs();
        setupZeroLine();
    }

    QCPGraph* graph = m_plot->addGraph();
    QColor color = QColor::fromHsl(QRandomGenerator::global()->bounded(360), 180, 140);
    graph->setPen(QPen(color, 1.4));
    graph->setAntialiased(true);

    QVector<double> xData, yData;
    for (const auto& pt : res.path) {
        xData << pt.x();
        yData << pt.y();
    }
    graph->setData(xData, yData);

    if (res.crossingCount > 0) {
        QCPGraph* crossings = m_plot->addGraph();
        crossings->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 9));
        crossings->setLineStyle(QCPGraph::lsNone);

        QVector<double> cx, cy;
        double prevY = res.path.first().y();
        double currX = 0.0;

        for (const auto& pt : res.path) {
            double currY = pt.y();
            if ((prevY * currY < 0) || (currY == 0 && prevY != 0)) {
                cx << currX;
                cy << currY;
            }
            prevY = currY;
            currX += m_config.stepH;
        }

        crossings->setData(cx, cy);
    }

    m_plot->rescaleAxes();

    m_plot->yAxis->scaleRange(1.2, m_plot->yAxis->range().center());
    m_plot->xAxis->scaleRange(1.1, m_plot->xAxis->range().center());

    m_plot->replot();
}

void Window::applyBentoStyle()
{
    this->setStyleSheet(R"(
        * {
            font-family: 'Segoe UI', 'Helvetica Neue', sans-serif;
            color: #4A2C2A;
        }

        QMainWindow, QWidget {
            background-color: #FFF8FA;
        }

        QLabel#headerLabel {
            font-size: 22px;
            font-weight: bold;
            color: #FF6B8E;
            letter-spacing: 1px;
            padding: 10px 0;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #FFF0F5, stop:1 #FFE4EC);
            border-bottom: 2px solid #FFD1DC;
        }

        QWidget#bentoCard {
            background-color: #FFFFFF;
            border-radius: 16px;
            border: 1px solid #FFE0E9;
            box-shadow: 0 4px 15px rgba(255, 105, 180, 0.08);
        }

        QLabel#cardTitle {
            font-size: 15px;
            font-weight: 700;
            color: #FF85A2;
            padding: 8px 12px;
            background: #FFF0F5;
            border-radius: 12px 12px 0 0;
        }

        QPushButton {
            background-color: #FF9BB5;
            color: white;
            border: none;
            border-radius: 12px;
            padding: 10px 18px;
            font-weight: 600;
            font-size: 14px;
        }

        QPushButton:hover {
            background-color: #FFADC2;
        }

        QPushButton:pressed {
            background-color: #FF85A2;
        }

        QPushButton:disabled {
            background-color: #E0E0E0;
            color: #A0A0A0;
        }

        QDoubleSpinBox, QSpinBox {
            background-color: #FFF5F7;
            border: 2px solid #FFD1DC;
            border-radius: 10px;
            padding: 6px 8px;
            font-size: 14px;
            color: #4A2C2A;
        }

        QDoubleSpinBox::up-button, QDoubleSpinBox::down-button,
        QSpinBox::up-button, QSpinBox::down-button {
            width: 20px;
            border: none;
            background: #FFE0E9;
        }

        QTextEdit#teLogs {
            background-color: #FFF8FA;
            border: 2px solid #FFE0E9;
            border-radius: 14px;
            padding: 12px;
            font-size: 13px;
            color: #5A3A38;
        }

        QLabel#probLabel {
            font-size: 20px;
            font-weight: bold;
            color: #FF6B8E;
            background: #FFF0F5;
            border-radius: 12px;
            padding: 12px;
            margin: 8px 0;
        }

        QCustomPlot {
            background-color: #FFFFFF;
            border: 1px solid #FFE0E9;
            border-radius: 12px;
        }
    )");
}

