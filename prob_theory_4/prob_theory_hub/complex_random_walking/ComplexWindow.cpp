#include "ComplexWindow.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include "../complex_random_walking/ConfigParser.hpp"

ComplexWindow::ComplexWindow(QWidget* parent) : QMainWindow(parent)
{
    buildUI();
    resize(1000, 700);
    applyGlassStyle();
    //qsrand(QTime::currentTime().msec());
}

void ComplexWindow::buildUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    //LEFT PANEl
    QWidget* sidePanel = new QWidget(this);
    sidePanel->setFixedWidth(300);
    sidePanel->setObjectName("sidePanel");
    QVBoxLayout* sideLayout = new QVBoxLayout(sidePanel);
    sideLayout->setContentsMargins(0, 0, 0, 0);
    sideLayout->setSpacing(15);

    // LEFT PANEL -> TITLE
    m_lblProjectTitle = new QLabel("PROBABILITY THEORY HUB", this);
    m_lblProjectTitle->setWordWrap(true);
    m_lblProjectTitle->setObjectName("projectTitle");
    m_lblProjectTitle->setAlignment(Qt::AlignCenter);
    sideLayout->addWidget(m_lblProjectTitle);

    //LEFT PANEL -> CONFIG
    m_cardConfig = new QFrame(this);
    m_cardConfig->setObjectName("configCard");
    QVBoxLayout* configLayout = new QVBoxLayout(m_cardConfig);

    QLabel* cfgHeader = new QLabel("CONFIGURATION", m_cardConfig);
    cfgHeader->setObjectName("cardHeader");

    btnImport = new QPushButton("Import JSON", m_cardConfig);
    btnImport->setObjectName("btnImport");

    m_lblConfigInfo = new QLabel("No configuration loaded", m_cardConfig);
    m_lblConfigInfo->setObjectName("configDisplay");
    m_lblConfigInfo->setWordWrap(true);

    configLayout->addWidget(cfgHeader);
    configLayout->addWidget(btnImport);
    configLayout->addWidget(m_lblConfigInfo);

    //LEFT PANEL -> CONTROL
    m_cardControl = new QFrame(this);
    m_cardControl->setObjectName("controlCard");
    QVBoxLayout* controlLayout = new QVBoxLayout(m_cardControl);

    QLabel* ctrlHeader = new QLabel("ENGINE CONTROL", m_cardControl);
    ctrlHeader->setObjectName("cardHeader");

    btnRun = new QPushButton("Run All", m_cardControl);
    btnRun->setObjectName("btnRun");

    btnStep = new QPushButton("Single Step", m_cardControl);
    btnStep->setObjectName("btnStep");

    btnReset = new QPushButton("Reset Engine", m_cardControl);
    btnReset->setObjectName("btnReset");

    btnCenter = new QPushButton("Center Camera", m_cardControl);
    btnCenter->setObjectName("btnCenter");

    controlLayout->addWidget(ctrlHeader);
    controlLayout->addWidget(btnRun);
    controlLayout->addWidget(btnStep);
    controlLayout->addWidget(btnReset);
    controlLayout->addWidget(btnCenter);

    //LEFT PANEL -> STATUS
    m_cardStatus = new QFrame(this);
    m_cardStatus->setObjectName("statusCard");
    QVBoxLayout* statusLayout = new QVBoxLayout(m_cardStatus);

    QLabel* statHeader = new QLabel("LIVE STATISTICS", m_cardStatus);
    statHeader->setObjectName("cardHeader");

    m_lblLivePos = new QLabel("Position: (0.00, 0.00)", m_cardStatus);
    m_lblLivePos->setObjectName("statLabel");

    m_lblProbability = new QLabel("P(return): 0.0000", m_cardStatus);
    m_lblProbability->setObjectName("statLabelHighlight");

    statusLayout->addWidget(statHeader);
    statusLayout->addWidget(m_lblLivePos);
    statusLayout->addWidget(m_lblProbability);

    sideLayout->addWidget(m_cardConfig);
    sideLayout->addWidget(m_cardControl);
    sideLayout->addWidget(m_cardStatus);
    sideLayout->addStretch();

    //RIGHT PANEL
    m_plot = new QCustomPlot(this);
    m_plot->setObjectName("mainPlot");
    setupPlot();

    mainLayout->addWidget(sidePanel);
    mainLayout->addWidget(m_plot, 1);

    connect(btnImport, &QPushButton::clicked, this, &ComplexWindow::onImportJson);
    connect(btnRun, &QPushButton::clicked, this, &ComplexWindow::onRunAll);
    connect(btnStep, &QPushButton::clicked, this, &ComplexWindow::onStep);
    connect(btnReset, &QPushButton::clicked, this, &ComplexWindow::onReset);
    connect(btnCenter, &QPushButton::clicked, this, &ComplexWindow::onCenterCamera);
}

ComplexWindow::~ComplexWindow()
{
    delete m_engine;
}

void ComplexWindow::onImportJson(){
    QString path = QFileDialog::getOpenFileName(this,
                                                "Open config : ",
                                                "",
                                                "JSON Files (*.json)");
    if (path.isEmpty()) return;

    ConfigParser_4::ParseResult result = ConfigParser_4::parseJson(path);

    if(!result.success)
    {
        QMessageBox::critical(
                this,
                tr("Parsing Error"),
                result.errorString
        );
        return;
    }

    m_config = &result.config;

    if (!m_engine)
    {
        m_engine = new WalkEngine(result.config, result.model);
    }
    else
    {
        m_engine->updateConfig(result.config);
        m_engine->setDistribution(result.model);
    }

    updateConfigInfo(path);
    onReset();
}
void ComplexWindow::updateConfigInfo(const QString& path) {
    if (!m_engine) return;

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    WalkConfig cfg = m_engine->getConfig();
    QString distType = obj["distribution"].toString().toUpper();

    QString html = "<b>Parameters</b><br>";
    html += QString("Count of trajectories (M): <b>%1</b><br>").arg(cfg.M);
    html += QString("Max Steps (K): <b>%1</b><br>").arg(cfg.K);
    html += QString("Length of step (rho): <b>%1</b><br>").arg(cfg.p);
    html += QString("Direction (n): <b>%1</b><br>").arg(cfg.n);

    html += "<br><b>Distribution</b><br>";
    html += QString("Type: <b>%1</b><br>").arg(distType.isEmpty() ? "DISCRETE/CUSTOM" : distType);

    if (obj.contains("p_dist")) {
        html += QString("Parameter p_dist: <b>%1</b><br>").arg(obj["p_dist"].toDouble());
    }

    if (obj.contains("weights")) {
        html += "Weights: <b>using</b><br>";
    }

    m_lblConfigInfo->setText(html);

    this->setWindowTitle("Walk Engine - " + QFileInfo(path).fileName());
}

void ComplexWindow::onRunAll(){
    if (!m_engine) {
        onImportJson();
        if (!m_engine) return;
    }

    m_engine->runAll();
    const auto& allRes = m_engine->getAllResults();

    m_plot->clearPlottables();
    m_plot->clearItems();
    m_plot->clearGraphs();

    for (const auto& res : allRes) {
        drawTrajectory(res);
    }

    m_plot->rescaleAxes();
    m_plot->replot();


    if (!allRes.empty()) {
        updateLiveStatus(allRes.back().points.back());
    }

    btnStep->setEnabled(false);
    btnRun->setEnabled(false);
}
void ComplexWindow::onStep(){

    m_plot->clearPlottables();
    m_plot->clearItems();

    if (!m_engine) {
        onImportJson();
        if (!m_engine) return;
    }

    int startTraj = m_engine->getCurrentTrajIndex();
    bool finished = false;

    while (!finished) {
        auto opt = m_engine->step();
        if (!opt.has_value()) {
            finished = true;
            break;
        }
        if (m_engine->getCurrentTrajIndex() > startTraj) {
            break;
        }
    }

    const auto& allRes = m_engine->getAllResults();
    if (!allRes.empty()) {
        drawTrajectory(allRes.back());
        updateLiveStatus(allRes.back().points.back());
        m_plot->rescaleAxes();
        m_plot->replot();
    }

    if (finished || allRes.size() >= m_engine->getConfig().M) {
        btnStep->setEnabled(false);
        btnRun->setEnabled(false);
    }
}
void ComplexWindow::onReset(){
    if (m_engine) {
        m_engine->reset();
    }

    m_plot->clearPlottables();
    m_plot->clearItems();

    m_plot->clearGraphs();
    m_plot->replot();

    m_lblLivePos->setText("🐾 Position: (0.00, 0.00)");
    m_lblProbability->setText("✨ P(Returns): 0.0000");

    btnStep->setEnabled(true);
    btnRun->setEnabled(true);
}
void ComplexWindow::onCenterCamera(){
    m_plot->rescaleAxes();
    m_plot->replot();
}
void ComplexWindow::setupPlot(){
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);
    m_plot->axisRect()->setupFullAxesBox(true);
    m_plot->xAxis->setLabel("Re");
    m_plot->yAxis->setLabel("Im");
    m_plot->xAxis->setRange(-5, 5);
    m_plot->yAxis->setRange(-5, 5);
    connect(m_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(m_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_plot->yAxis2, SLOT(setRange(QCPRange)));
    m_plot->axisRect()->setRangeZoomAxes(m_plot->xAxis, m_plot->yAxis);
    m_plot->axisRect()->setRangeDragAxes(m_plot->xAxis, m_plot->yAxis);
}
void ComplexWindow::drawTrajectory(const TrajectoryResult& res){
    QCPCurve* pathCurve = new QCPCurve(m_plot->xAxis, m_plot->yAxis);

    QColor color = QColor::fromHsl(rand() % 360, 150, 150);
    pathCurve->setPen(QPen(color, 1.5));

    int n = res.points.size();
    QVector<double> t(n), x(n), y(n);

    for (int i = 0; i < n; ++i) {
        t[i] = i;
        x[i] = res.points[i].real();
        y[i] = res.points[i].imag();
    }

    pathCurve->setData(t, x, y);

    if (res.return_to_origin) {
        QCPGraph* marker = m_plot->addGraph();
        marker->setLineStyle(QCPGraph::lsNone);
        marker->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::yellow, 10));
        marker->addData(0, 0);
    }
}
void ComplexWindow::updateLiveStatus(const std::complex<double>& pos){
    m_lblLivePos->setText(QString("🐾 Position: (<b>%1</b>, <b>%2</b>)")
                                  .arg(pos.real(), 0, 'f', 2)
                                  .arg(pos.imag(), 0, 'f', 2));

    if (m_engine) {
        double prob = m_engine->getEmpiricalProbability();
        int returns = m_engine->getSuccessfulReturnsCount();
        int total = m_engine->getAllResults().size();

        m_lblProbability->setText(QString("✨ P(return): <b style='color:#8AB4F8;'>%1</b> (Returns: %2/%3)")
                                          .arg(prob, 0, 'f', 4).arg(returns).arg(total));
    }
}

void ComplexWindow::applyGlassStyle() {
    QFont mainFont("Segoe UI", 14);
    mainFont.setStyleStrategy(QFont::PreferAntialias);
    this->setFont(mainFont);

    m_plot->setBackground(QBrush(QColor("#F8FAFC")));
    m_plot->axisRect()->setBackground(QBrush(QColor("#FFFFFF")));

    QPen verySoftBlue(QColor("#BAE6FD"), 2);
    m_plot->xAxis->setBasePen(verySoftBlue);
    m_plot->yAxis->setBasePen(verySoftBlue);
    m_plot->xAxis->grid()->setPen(QPen(QColor("#E0F2FE"), 1, Qt::DotLine));
    m_plot->yAxis->grid()->setPen(QPen(QColor("#E0F2FE"), 1, Qt::DotLine));

    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #F0F9FF;
        }
        QFrame#configCard, QFrame#controlCard, QFrame#statusCard {
            background-color: rgba(255, 255, 255, 0.97);
            border-radius: 22px;
            border: 2px solid #E0F2FE;
            padding: 16px;
        }

        QLabel#projectTitle {
            font-size: 23px;
            font-weight: 700;
            color: #0C7A9E;
            padding: 8px 0;
        }

        QLabel#cardHeader {
            font-size: 14px;
            font-weight: 700;
            color: #67B8E3;
            letter-spacing: 0.6px;
        }

        QLabel {
            color: #334155;
            font-size: 14px;
        }

        QLabel#statLabel {
            font-size: 15px;
            font-weight: 600;
            color: #1E2937;
        }
        QLabel#statLabelHighlight {
            font-size: 17px;
            font-weight: 700;
            color: #0C7A9E;
            background-color: #F0FAFF;
            border-radius: 12px;
            padding: 9px 16px;
        }

        QPushButton {
            background-color: #F8FAFC;
            color: #0C7A9E;
            border: 2px solid #BAE6FD;
            border-radius: 18px;
            padding: 12px 20px;
            font-size: 14px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #E0F2FE;
            border-color: #67B8E3;
        }
        QPushButton:pressed {
            background-color: #BAE6FD;
        }
        QPushButton:disabled {
            color: #94A3B8;
            border-color: #E0F2FE;
        }

        QLabel#configDisplay {
            font-size: 13.5px;
            line-height: 1.55;
            color: #475569;
        }
    )");
}