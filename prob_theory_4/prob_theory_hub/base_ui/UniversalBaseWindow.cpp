//
// Created by mvideo on 06/03/26.
//

#include "UniversalBaseWindow.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include <QRandomGenerator>
#include <QIntValidator>

#include<algorithm>

#include "../external/qcustomplot.h"

void applyBentoStyle(QWidget* window) {
    window->setStyleSheet(R"(
        * {
            font-family: 'JetBrains Mono';
            color: #7D5A5A;
        }

        QMainWindow, QWidget#centralObject {
            background-color: #FFF5F7;
        }

        QLabel#panelHeader, QLabel#titleLabel, QLabel#headerLabel {
            font-family: 'Ubuntu', sans-serif;
            font-size: 22px;
            font-weight: 900;
            color: #FF85A2;
            text-transform: uppercase;
            letter-spacing: 1.5px;
            margin-bottom: 10px;
        }

        QGroupBox, input_panel, visual_panel, statistics_panel {
            background-color: #FFFFFF;
            border-radius: 30px;
            border: none;
            padding: 15px;
        }

        QTableWidget {
            background-color: white;
            border: none;
            gridline-color: #FFF5F7;
            outline: 0;
        }
        QHeaderView::section {
            background-color: #FFF5F7;
            color: #FF85A2;
            padding: 8px;
            border: none;
            font-weight: bold;
            font-size: 12px;
        }

        QSpinBox, QDoubleSpinBox {
            background-color: #FFF5F7;
            border: 2px solid #FFD1DC;
            border-radius: 12px;
            padding: 8px;
            font-size: 14px;
            color: #7D5A5A;
        }

        QSpinBox::up-button, QSpinBox::down-button,
        QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
            width: 0px;
            height: 0px;
            border: none;
            background: transparent;
        }

        QProgressBar {
            background-color: #FFF5F7;
            border-radius: 12px;
            text-align: center;
            color: #FF85A2;
            font-weight: 800;
            border: 1px solid #FFE5EC;
            height: 22px;
        }
        QProgressBar::chunk {
            background-color: #FFC2D1;
            border-radius: 11px;
        }

        QTextEdit {
            background-color: #FFFBFC;
            border: 2px solid #FFF5F7;
            border-radius: 20px;
            padding: 15px;
            font-size: 16px;
            font-weight: 600;
            color: #FF85A2;
        }

        QPushButton {
            background-color: #FFC2D1;
            color: white;
            border-radius: 15px;
            padding: 12px;
            font-weight: 800;
            border: none;
        }
        QPushButton:hover {
            background-color: #FFADC2;
        }
        QPushButton:pressed {
            background-color: #FF85A2;
        }

        QPushButton:disabled {
            background-color: #D0D0D0;
            color: #8E8E8E;
            border: 1px solid #C0C0C0;
        }

        QSlider::groove:horizontal {
            height: 6px;
            background: #FFE5EC;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #FF85A2;
            border: 2px solid white;
            width: 16px;
            height: 16px;
            margin: -5px 0;
            border-radius: 8px;
        }
    )");

}
UniversalBaseWindow::UniversalBaseWindow(
        const QString& title,
        const QMap<QString,
        ControlSchema>& schema,
        QWidget *parent) : QMainWindow(parent)
{
    QWidget* central_widget = new QWidget(this);
    central_widget->setObjectName("centralObject");
    setCentralWidget(central_widget);

    QHBoxLayout* mainLayout = new QHBoxLayout(central_widget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    buildUI(title, schema);
    setupConnections();
    applyBentoStyle(central_widget);
}

void UniversalBaseWindow::setupConnections() {
    if(m_btnStart)
    {
        connect(m_btnStart, &QPushButton::clicked, this, &UniversalBaseWindow::onStartTriggered);
    }
    if (m_btnStep)
    {
        connect(m_btnStep, &QPushButton::clicked, this, &UniversalBaseWindow::onSingleStep);
    }
    if (m_btnBatch)
    {
        connect(m_btnBatch, &QPushButton::clicked, this, &UniversalBaseWindow::onBatchStep);
    }
    if (m_btnReset)
    {
        connect(m_btnReset, &QPushButton::clicked, this, &UniversalBaseWindow::onResetStep);
    }
    if (m_btnPrev)
    {
        connect(m_btnPrev, &QPushButton::clicked, this, &UniversalBaseWindow::onPrevPage);
    }
    if (m_btnNext)
    {
        connect(m_btnNext, &QPushButton::clicked, this, &UniversalBaseWindow::onNextPage);
    }
}

UniversalBaseWindow::~UniversalBaseWindow()
{

}

void UniversalBaseWindow::buildUI(const QString& title, const QMap<QString, ControlSchema>& schema)
{

    QWidget* leftPanel = new QWidget(this);
    leftPanel->setObjectName("leftPanel");
    leftPanel->setFixedWidth(380);

    m_leftLayout = new QVBoxLayout(leftPanel);
    m_leftLayout->setContentsMargins(12, 12, 12, 12);
    m_leftLayout->setSpacing(10);

    QLabel* titleLabel = new QLabel(title);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_leftLayout->addWidget(titleLabel);

    //SCROLL
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setObjectName("paramScrollArea");
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* scrollContent = new QWidget();
    scrollContent->setObjectName("scrollContent");
    QFormLayout* formLayout = new QFormLayout(scrollContent);
    formLayout->setLabelAlignment(Qt::AlignLeft);

    for (auto it = schema.begin(); it != schema.end(); ++it)
    {
        QWidget* input = createInputWidget(it.value(), it.key());
        m_inputs[it.key()] = input;
        formLayout->addRow(it.value().label, input);
    }

    scrollArea->setWidget(scrollContent);
    m_leftLayout->addWidget(scrollArea);


    QGroupBox* controlGroup = new QGroupBox("Control");
    controlGroup->setObjectName("controlGroup");
    QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);

    m_btnStep = new QPushButton("Single Step");
    m_btnStep->setObjectName("stepBtn");
    m_btnStep->setMinimumHeight(45);
    controlLayout->addWidget(m_btnStep);

    m_btnBatch = new QPushButton("Multiple Step");
    m_btnBatch->setObjectName("batchBtn");
    m_btnBatch->setMinimumHeight(45);
    controlLayout->addWidget(m_btnBatch);

    m_btnStart = new QPushButton("Running all");
    m_btnStart->setObjectName("startBtn");
    m_btnStart->setMinimumHeight(45);
    controlLayout->addWidget(m_btnStart);

    m_btnReset = new QPushButton("Reset");
    m_btnReset->setObjectName("resetBtn");
    m_btnReset->setMinimumHeight(45);
    controlLayout->addWidget(m_btnReset);

    m_leftLayout->addWidget(controlGroup);

    QGroupBox* statsGroup = new QGroupBox("Results");
    m_statsLayout = new QFormLayout(statsGroup);
    m_statsLayout->setSpacing(10);
    m_leftLayout->addWidget(statsGroup);

    m_leftLayout->addStretch();

    QWidget* rightPanel = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    m_plot = new QCustomPlot();
    m_plot->setObjectName("mainPlot");
    rightLayout->addWidget(m_plot, 4);
/*
    QWidget* paginPanel = new QWidget();
    paginPanel->setObjectName("paginationPanel");
    QHBoxLayout* paginLayout = new QHBoxLayout(paginPanel);
*/

    QFrame* paginFrame = new QFrame();
    paginFrame->setObjectName("paginationContainer");
    m_paginationContainer = paginFrame;

    QHBoxLayout* paginLayout = new QHBoxLayout(paginFrame);
    paginLayout->setContentsMargins(5, 0, 5, 0);

    m_btnPrev = new QPushButton("<<");
    m_pageLabel = new QLabel("Result 0 / 0");
    m_btnNext = new QPushButton(">>");
    m_pageLabel->setObjectName("pageLabel");

    paginLayout->addWidget(m_btnPrev);
    paginLayout->addStretch();
    paginLayout->addWidget(m_pageLabel);
    paginLayout->addStretch();
    paginLayout->addWidget(m_btnNext);

    rightLayout->addWidget(paginFrame);

    m_log = new QTextEdit();
    m_log->setObjectName("consoleLog");
    m_log->setReadOnly(true);
    m_log->setMaximumHeight(150);
    rightLayout->addWidget(m_log, 1);

    if (centralWidget() && centralWidget()->layout())
    {
        QHBoxLayout* mainLayout = qobject_cast<QHBoxLayout*>(centralWidget()->layout());
        mainLayout->addWidget(leftPanel);
        mainLayout->addWidget(rightPanel);
    }
}

QWidget* UniversalBaseWindow::createInputWidget(const ControlSchema& sch, const QString& key)
{
    QWidget* widget = nullptr;
    switch(sch.type)
    {
        case ControlType::DoubleSpin:
        {
            auto* dsb = new QDoubleSpinBox();
            dsb->setRange(sch.min, sch.max);
            dsb->setValue(sch.defaultValue.toDouble());
            dsb->setDecimals(sch.decimals);

            double step = qMax(0.0001, (sch.max - sch.min) / 100.0);
            dsb->setSingleStep(step);

            if (sch.useScientificNotation)
            {
                dsb->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
            }

            dsb->setFocusPolicy(Qt::WheelFocus);
            widget = dsb;
            break;
        }

        case ControlType::IntSpin:
        {
            auto* sb = new QSpinBox();
            sb->setRange(qRound(sch.min), qRound(sch.max));
            sb->setValue(sch.defaultValue.toInt());
            sb->setFocusPolicy(Qt::WheelFocus);
            widget = sb;
            break;
        }

        case ControlType::BigIntSpin:
        {
            auto* bsb = new QDoubleSpinBox();
            bsb->setDecimals(0);
            bsb->setRange(sch.min, sch.max);
            bsb->setValue(sch.defaultValue.toDouble());
            bsb->setSingleStep(1000.0);
            bsb->setFocusPolicy(Qt::WheelFocus);
            widget = bsb;
            break;
        }

        case ControlType::ComboBox:
        {
            auto* combo = new QComboBox();
            if (!sch.options.isEmpty())
            {
                combo->addItems(sch.options);
                combo->setCurrentIndex(0);
            }
            else
            {
                combo->addItem(" No options ");
            }
            combo->setFocusPolicy(Qt::WheelFocus);
            widget = combo;
            break;
        }

        case ControlType::Distribution:
        {
            QWidget* panel = createDistributionPanel(key, sch.defaultDist);
            widget = panel;
            break;
        }

        default:
        {
            auto* le = new QLineEdit();
            le->setText(sch.defaultValue.toString());
            widget = le;
            break;
        }
    }

    if(widget)
    {
        widget->setObjectName(key);
        widget->setToolTip(QString("%1 (%2)").arg(sch.label).arg(key));

        widget->setMinimumWidth(140);
        widget->setMaximumHeight(220);

        m_inputs[key] = widget;
    }
    return widget;
}

QJsonObject UniversalBaseWindow::collectCurrentConfig() const
{
    QJsonObject config;

    for (auto it = m_inputs.constBegin(); it != m_inputs.constEnd(); ++it)
    {
        QString key = it.key();
        QWidget* w = it.value();

        if (key.endsWith("_a")
            || key.endsWith("_b")
            || key.endsWith("_p")
            || key.endsWith("_n")
            || key.endsWith("_max")
            || key.endsWith("_c"))
            continue;

        if (auto* dsb = qobject_cast<QDoubleSpinBox*>(w)) config[key] = dsb->value();
        else if (auto* sb = qobject_cast<QSpinBox*>(w)) config[key] = sb->value();
        else if (auto* cb = qobject_cast<QComboBox*>(w)) config[key] = cb->currentIndex();
        else if (auto* le = qobject_cast<QLineEdit*>(w)) config[key] = le->text();
    }
    return config;
}

void UniversalBaseWindow::onStartTriggered()
{
    QJsonObject config = collectCurrentConfig();

    if(config.isEmpty())
    {
        log("ERROR : configuration is empty");
        return;
    }

    log("Start calculation . . . ");
    runCalculation(config);
    log("Calculation is finished!");
}
void UniversalBaseWindow::onSingleStep()
{
    log("Processing single step");
}
void UniversalBaseWindow::onBatchStep()
{
    log("Processing multiple steps");
}

void UniversalBaseWindow::onResetStep()
{
    if (m_log) m_log->clear();
    if (m_plot) {
        m_plot->clearGraphs();
        m_plot->replot();
    }
}

void UniversalBaseWindow::onPrevPage()
{
    if (m_currentPage <= 0) return;

    m_currentPage--;

    showResult(m_currentPage);
    updatePagination(m_currentPage + 1, getTotalResultsCount());
    log(QString("Переход на предыдущую страницу: %1").arg(m_currentPage + 1));
}
void UniversalBaseWindow::onNextPage()
{
    int total = getTotalResultsCount();
    if (m_currentPage + 1 >= total) return;

    m_currentPage++;

    showResult(m_currentPage);
    updatePagination(m_currentPage + 1, total);
    log(QString("Переход на следующую страницу: %1").arg(m_currentPage + 1));
}

int UniversalBaseWindow::getTotalResultsCount() const
{
    return std::max({m_trajectories.size(),
                 m_complexPaths.size(),
                 m_stepFunctions.size()});
}

void UniversalBaseWindow::onExportConfig()
{
    log("Export config");
}
void UniversalBaseWindow::onImportConfig()
{
    log("<b>Starting import...</b>");
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Open Config File", "", "JSON Files (*.json);;All Files (*)");

    if(filename.isEmpty())
    {
        log("Import cancelled by user.");
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        log("<span style='color:red;'>Error:</span> Could not open file for reading.");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (doc.isNull()) {
        log("<span style='color:red;'>JSON Error:</span> " + parseError.errorString());
        return;
    }

    QJsonObject root = doc.object();

    applyConfig(root);

    log("<span style='color:green;'>Import successful!</span> File: " + filename);
}

void UniversalBaseWindow::applyConfig(const QJsonObject& config)
{
    log("<b>Loading configuration from JSON...</b>");
    for (auto it = config.begin(); it != config.end(); ++it)
    {
        QString key = it.key();
        QJsonValue val = it.value();

        if (m_inputs.contains(key))
        {
            QWidget* w = m_inputs[key];
            if (auto* dsb = qobject_cast<QDoubleSpinBox*>(w)) {
                dsb->setValue(val.toDouble());
            } else if (auto* sb = qobject_cast<QSpinBox*>(w)) {
                sb->setValue(val.toInt());
            }
            continue;
        }

        else if (val.isObject() && m_distGroups.contains(key)) {
            QJsonObject distObj = val.toObject();
            if (distObj.contains("law")) {
                QString lawKey = key + "_law";
                if (m_inputs.contains(lawKey)) {
                    if (auto* cb = qobject_cast<QComboBox*>(m_inputs[lawKey]))
                        cb->setCurrentIndex(distObj["law"].toInt());
                }
            }
            QStringList params = {"a", "b", "p", "trials", "c", "max"};
            for (const QString& pName : params) {
                if (distObj.contains(pName)) {
                    QString widgetKey = key + "_" + pName;
                    if (m_inputs.contains(widgetKey)) {
                        if (auto* dsb = qobject_cast<QDoubleSpinBox*>(m_inputs[widgetKey]))
                            dsb->setValue(distObj[pName].toDouble());
                        else if (auto* sb = qobject_cast<QSpinBox*>(m_inputs[widgetKey]))
                            sb->setValue(distObj[pName].toInt());
                    }
                }
            }
        }
    }
    log("<span style='color:green;'>UI successfully updated with data from the file.</span>");
}

void UniversalBaseWindow::onDistributionChanged(const QString& key, int index)
{
    log(QString("Law of distribution for %1 changed to %2").arg(key).arg(index));
}
void UniversalBaseWindow::onPageButtonClicked()
{
    log("Swiping to the next page . . . ");
}

void UniversalBaseWindow::log(const QString& message)
{
    QString ts = QTime::currentTime().toString("[hh:mm:ss.zzz]");
    m_log->append(ts + " " + message);
    m_log->ensureCursorVisible();
}

void UniversalBaseWindow::clearPlot()
{
    if (m_plot) {
        m_plot->clearGraphs();
        m_plot->clearItems();
        m_plot->replot();
    }
}

double UniversalBaseWindow::getDouble(const QString& key) const
{
    QWidget* w = m_inputs.value(key, nullptr);
    if(!w) return 0.0;
    if(auto* dsb = qobject_cast<QDoubleSpinBox*>(w))
    {
        return dsb->value();
    }
    return 0.0;
}

qint64 UniversalBaseWindow::getBigInt(const QString& key) const
{
    QWidget* w = m_inputs.value(key, nullptr);
    if (!w) return 0;

    if (auto* dsb = qobject_cast<QDoubleSpinBox*>(w))
    {
        return static_cast<qint64>(dsb->value());
    }

    if (auto* sb = qobject_cast<QSpinBox*>(w))
    {
        return sb->value();
    }

    return 0;
}
int UniversalBaseWindow::getInt(const QString& key) const
{
    QWidget* w = m_inputs.value(key, nullptr);
    if (!w) return 0;

    if (auto* sb = qobject_cast<QSpinBox*>(w))
    {
        return sb->value();
    }

    if (auto* dsb = qobject_cast<QDoubleSpinBox*>(w))
    {
        return static_cast<int>(dsb->value());
    }

    return 0;
}
QString UniversalBaseWindow::getString(const QString& key) const
{
    QWidget* w = m_inputs.value(key, nullptr);
    if (!w) return QString();

    if (auto* cb = qobject_cast<QComboBox*>(w))
    {
        return cb->currentText();
    }

    if (auto* le = qobject_cast<QLineEdit*>(w))
    {
        return le->text();
    }

    return QString();
}
DistributionLaw UniversalBaseWindow::getDistributionLaw(const QString& key) const
{
    QWidget* w = m_inputs.value(key, nullptr);
    if(!w) return DistributionLaw::Uniform;

    if(auto* cb = qobject_cast<QComboBox*>(w))
    {
        return static_cast<DistributionLaw>(cb->currentIndex());
    }
    return DistributionLaw::Uniform;
}
DistParameters UniversalBaseWindow::getDistParams(const QString& key) const
{
    DistParameters params;
    params.law = getDistributionLaw(key);

    if (m_inputs.contains(key + "_p")) params.p = getDouble(key + "_p");
    if (m_inputs.contains(key + "_n")) params.trials = getInt(key + "_n");
    if (m_inputs.contains(key + "_a")) params.a = getDouble(key + "_a");
    if (m_inputs.contains(key + "_b")) params.b = getDouble(key + "_b");

    return params;
}

QWidget* UniversalBaseWindow::createDistributionPanel(const QString& key, const DistParameters& defaults)
{
    QGroupBox* group = new QGroupBox("Params of distribution");
    group->setObjectName(key + "_dist_group");

    m_distGroups[key] = group;

    QBoxLayout* layout = new QVBoxLayout(group);

    QComboBox* lawCombo = new QComboBox();
    lawCombo->addItems({"Uniform", "Binomial", "Geometric", "Triangle"});
    lawCombo->setCurrentIndex(static_cast<int>(defaults.law));
    layout->addWidget(lawCombo);

    QStackedWidget* stack = new QStackedWidget();
    layout->addWidget(stack);
    
    QWidget* pageUniform = new QWidget();
    QFormLayout* lUniform = new QFormLayout(pageUniform);
    auto* sbA = new QDoubleSpinBox(); sbA->setRange(-1e9, 1e9); sbA->setValue(defaults.a);
    auto* sbB = new QDoubleSpinBox(); sbB->setRange(-1e9, 1e9); sbB->setValue(defaults.b);
    lUniform->addRow("a (low):", sbA);
    lUniform->addRow("b (high):", sbB);
    stack->addWidget(pageUniform);

    QWidget* pageBinom = new QWidget();
    QFormLayout* lBinom = new QFormLayout(pageBinom);
    auto* sbN = new QSpinBox(); sbN->setRange(1, 1000000); sbN->setValue(defaults.trials);
    auto* sbP = new QDoubleSpinBox(); sbP->setRange(0, 1); sbP->setDecimals(4); sbP->setValue(defaults.p);
    lBinom->addRow("n (trials):", sbN);
    lBinom->addRow("p (success):", sbP);
    stack->addWidget(pageBinom);

    QWidget* pageGeom = new QWidget();
    QFormLayout* lGeom = new QFormLayout(pageGeom);
    auto* sbPg = new QDoubleSpinBox(); sbPg->setRange(0.0001, 1); sbPg->setValue(defaults.p);
    auto* sbMaxG = new QSpinBox(); sbMaxG->setRange(10, 100000); sbMaxG->setValue(defaults.maxValue);
    lGeom->addRow("p (success):", sbPg);
    lGeom->addRow("Max : ", sbMaxG);
    stack->addWidget(pageGeom);

    QWidget* pageTri = new QWidget();
    QFormLayout* lTri = new QFormLayout(pageTri);
    auto* sbAt = new QDoubleSpinBox(); sbAt->setRange(-1e9, 1e9); sbAt->setValue(defaults.a);
    auto* sbCt = new QDoubleSpinBox(); sbCt->setRange(-1e9, 1e9); sbCt->setValue(defaults.c);
    auto* sbBt = new QDoubleSpinBox(); sbBt->setRange(-1e9, 1e9); sbBt->setValue(defaults.b);
    lTri->addRow("a (left):", sbAt);
    lTri->addRow("c (mode):", sbCt);
    lTri->addRow("b (right):", sbBt);
    stack->addWidget(pageTri);


    m_inputs[key] = lawCombo;
    m_inputs[key + "_a"] = sbA;
    m_inputs[key + "_b"] = sbB;
    m_inputs[key + "_n"] = sbN;
    m_inputs[key + "_p"] = sbP;
    m_inputs[key + "_max"] = sbMaxG;
    m_inputs[key + "_c"] = sbCt;

    connect(lawCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            stack,
            &QStackedWidget::setCurrentIndex);

    connect(lawCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            [this, key](int idx)
            {onDistributionChanged(key, idx);});

    stack->setCurrentIndex(static_cast<int>(defaults.law));
    return group;
}

void UniversalBaseWindow::setStat(const QString& key, const QVariant& value, const QString& unit)
{
    if(!m_stats.contains(key))
    {
        QLabel* valueLabel = new QLabel("—");
        valueLabel->setObjectName(key + "_valueLabel");
        valueLabel->setStyleSheet("font-weight: bold; color: #2c3e50; font-size: 11pt;");

        if (m_statsLayout) {
            m_statsLayout->addRow(key + ":", valueLabel);
        }

        m_stats[key] = valueLabel;
    }
    if(m_stats.contains(key))
    {
        QString text;
        if(value.userType() == QMetaType::Double || value.userType() == QMetaType::Float)
        {
            double val = value.toDouble();
            if (qAbs(val) < 0.001 && val != 0)
            {
                text = QString::number(val, 'e', 2);
            }
            else
            {
                text = QString::number(val, 'f', 4);
            }
        }
        else
        {
            text = value.toString();
        }

        if (!unit.isEmpty()) text += " " + unit;
        m_stats[key]->setText(text);
    }
    else
    {
        log(QString("<span style='color:red;'>Stat Error:</span> Key '%1' not found in UI").arg(key));
        return;
    }
}

void UniversalBaseWindow::clearResults()
{
    clearPlot();
    m_log->clear();
    m_trajectories.clear();
    m_complexPaths.clear();
    m_stepFunctions.clear();
    m_resultDescriptions.clear();
    m_currentPage = 0;
    updatePagination(0, 0);
    setPaginationVisible(false);
    log("All results was cleared");
}
void UniversalBaseWindow::addTrajectory(const QVector<QPointF>& traj, const QString& description)
{
    if(traj.isEmpty())
    {
        log("Try to add empty trajectories - ignore");
        return;
    }

    m_trajectories.append(traj);

    int currentTotal = std::max({(int)m_trajectories.size(), (int)m_complexPaths.size(), (int)m_stepFunctions.size()});

    m_resultDescriptions.append(description.isEmpty()
                                ? QString("Trajectory %1").arg(currentTotal)
                                : description);

    log(QString("Add trajectory #%1 (%2 points)%3")
                .arg(m_trajectories.size())
                .arg(traj.size())
                .arg(description.isEmpty() ? "" : " — " + description));

    setResultCount(currentTotal);
}

void UniversalBaseWindow::addComplexTrajectory(const QVector<std::complex<double>>& path, const QString& desc)
{
    if(path.isEmpty())
    {
        log("Try to add empty trajectories - ignore");
        return;
    }

    QVector<QPointF> converted(path.size());

    for (int i = 0; i < path.size(); ++i)
    {
        converted[i] = QPointF(path[i].real(), path[i].imag());
    }

    m_complexPaths.append(converted);

    int total = std::max({(int)m_trajectories.size(), (int)m_complexPaths.size(), (int)m_stepFunctions.size()});

    m_resultDescriptions.append(desc.isEmpty()
                ? QString("Complex way %1").arg(m_complexPaths.size())
                : desc);

    log(QString("Add complex path #%1 (%2 points)%3")
                .arg(m_complexPaths.size())
                .arg(path.size())
                .arg(desc.isEmpty() ? "" : " — " + desc));

    if (total == 1)
    {
        showResult(0);
    }
    else
    {
        updatePagination(m_currentPage + 1, total);
    }
}

void UniversalBaseWindow::addStepFunction(const QVector<double>& heights, const QString& desc)
{
    if(heights.isEmpty())
    {
        log("Try to add empty step function - ignores");
        return;
    }
    m_stepFunctions.append(heights);
    m_resultDescriptions.append(desc.isEmpty()
                ? QString("Step %1").arg(m_stepFunctions.size())
                : desc);

    log(QString("Add step function #%1 (%2 levels)%3")
                .arg(m_stepFunctions.size())
                .arg(heights.size())
                .arg(desc.isEmpty() ? "" : " — " + desc));

    int total = std::max({(int)m_trajectories.size(),
                          (int)m_complexPaths.size(),
                          (int)m_stepFunctions.size()});

    showResult(total - 1);
}

void UniversalBaseWindow::setResultCount(int count)
{
    int total = std::max({m_trajectories.size(),
                          m_complexPaths.size(),
                          m_stepFunctions.size()});

    if (m_currentPage >= total)
    {
        m_currentPage = std::max(0, total - 1);
    }

    updatePagination(m_currentPage + 1, total);
    setPaginationVisible(total > 1);
}

void UniversalBaseWindow::showResult(int index)
{
    int total = std::max({(int)m_trajectories.size(),
                          (int)m_complexPaths.size(),
                          (int)m_stepFunctions.size()});
    if (index < 0 || index >= total)
    {
        log(QString("Result #%1 wasnt found").arg(index + 1));
        return;
    }

    m_currentPage = index;
    clearPlot();

    if (m_plot)
    {
        m_plot->xAxis->setLabel("X");
        m_plot->yAxis->setLabel("Y");
    }

    QString desc = m_resultDescriptions.value(index, "No name");
    bool plotted = false;

    if (index < m_trajectories.size())
    {
        const auto& traj = m_trajectories[index];
        QVector<double> x, y;
        x.reserve(traj.size()); y.reserve(traj.size());
        for (const QPointF& p : traj) {x << p.x(); y << p.y();}

        drawLine(x, y, QString("trajectory %1").arg(index + 1));
        plotted = true;
    }

    if (index < m_complexPaths.size())
    {
        const auto& path = m_complexPaths[index];
        QVector<double> re, im;

        re.reserve(path.size());
        im.reserve(path.size());

        for (const QPointF& p : path)
        {
            re << p.x();
            im << p.y();
        }

        drawLine(re, im, QString("Complex path %1").arg(index + 1), Qt::darkRed, 2);

        m_plot->xAxis->setLabel("Re");
        m_plot->yAxis->setLabel("Im");

        plotted = true;
    }

    if (index < m_stepFunctions.size())
    {
        const auto& heights = m_stepFunctions[index];
        QVector<double> x, y;

        x.reserve(heights.size() * 2);
        y.reserve(heights.size() * 2);

        for (int i = 0; i < heights.size(); ++i)
        {
            x << i << i + 1;
            y << heights[i] << heights[i];
        }

        drawLine(x, y, QString("Step %1").arg(index + 1), Qt::darkGreen, 2);
        plotted = true;
    }

    if (plotted)
    {
        log(QString("<b>Show results №%1:</b> %2").arg(index + 1).arg(desc));
    }

    updatePagination(index + 1, total);
    onPageChanged(index);
}

void UniversalBaseWindow::setPaginationVisible(bool visible)
{
    /*
    if (m_btnPrev) m_btnPrev->setVisible(visible);
    if (m_btnNext) m_btnNext->setVisible(visible);
    if (m_pageLabel) m_pageLabel->setVisible(visible);
    if (m_pageSpin) m_pageSpin->setVisible(visible);
     */

    if (m_paginationContainer)
    {
        m_paginationContainer->setVisible(visible);
    }
}

void UniversalBaseWindow::updatePagination(int current, int total)
{
    if(m_pageLabel)
    {
        m_pageLabel->setText(QString("Result %1 / %2").arg(current).arg(total));
    }
    if (m_btnPrev) m_btnPrev->setEnabled(current > 1);
    if (m_btnNext) m_btnNext->setEnabled(current < total && total > 0);
}

void UniversalBaseWindow::drawLine(
        const QVector<double>& x,
        const QVector<double>& y,
        const QString& name,
        QColor color, int width)
{
    if (!m_plot || x.isEmpty() || x.size() != y.size())
    {
        log("<span style='color:orange;'>drawLine:</span> data is empty or incorrect");
        return;
    }

    QCPGraph* graph = m_plot->addGraph();
    graph->setData(x, y);
    graph->setName(name);

    QPen pen(color);
    pen.setWidth(width);
    pen.setStyle(Qt::SolidLine);
    graph->setPen(pen);

    if (m_plot->legend && !m_plot->legend->visible())
    {
        m_plot->legend->setVisible(true);
        m_plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    }

    graph->rescaleAxes(true);

    m_plot->xAxis->scaleRange(1.05, m_plot->xAxis->range().center());
    m_plot->yAxis->scaleRange(1.05, m_plot->yAxis->range().center());

    m_plot->replot();
}
void UniversalBaseWindow::drawBars(
        const QVector<double>& x,
        const QVector<double>& y,
        const QString& name,
        QColor color)
{
    if (!m_plot || x.isEmpty() || x.size() != y.size())
    {
        log("<span style='color:orange;'>drawLine:</span> data is empty or incorrect");
        return;
    }

    QCPBars* bars = new QCPBars(m_plot->xAxis, m_plot->yAxis);
    bars->setData(x, y);
    bars->setName(name);

    bars->setPen(QPen(color.darker(130), 1));
    bars->setBrush(QBrush(color.lighter(120)));

    double width = 0.8;
    if (x.size() > 1)
    {
        double diff = qAbs(x[1] - x[0]);
        width = (diff > 0) ? diff * 0.8 : 0.8;
    }

    bars->setWidth(width);

    if (m_plot->legend && !m_plot->legend->visible())
    {
        m_plot->legend->setVisible(true);
        m_plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
    }

    m_plot->rescaleAxes();
    m_plot->yAxis->scaleRange(1.1, m_plot->yAxis->range().center());
    m_plot->replot();
}

void UniversalBaseWindow::drawComplexTrajectory(
        const QVector<std::complex<double>>& path,
        const QString& name,
        QColor color)
{
    if (path.isEmpty() || !m_plot) {
        log("drawComplexTrajectory: path is empty or graph is not initialized");
        return;
    }

    QVector<double> re, im;
    re.reserve(path.size());
    im.reserve(path.size());

    for (const auto& z : path)
    {
        re << z.real();
        im << z.imag();
    }

    drawLine(re, im, name, color, 2);

    m_plot->xAxis->setLabel("Re");
    m_plot->yAxis->setLabel("Im");

    m_plot->replot();
}