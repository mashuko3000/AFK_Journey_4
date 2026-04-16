//
// Created by maria on 15.04.2026.
//

#include "MainWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QPainter>
#include <QApplication>

WalkView::WalkView(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(150);
}

void WalkView::setRange(double minX, double maxX) {
    double margin = (maxX - minX) * 0.1;
    if (margin < 1.0) margin = 1.0;
    m_minX = minX - margin;
    m_maxX = maxX + margin;
    update();
}

void WalkView::setPointPosition(double x) {
    m_currentPos = x;
    update();
}

void WalkView::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    int y = h / 2;

    p.setPen(QPen(Qt::black, 2));
    p.drawLine(10, y, w - 10, y);

    auto toScreen = [&](double x) {
        double range = m_maxX - m_minX;
        if (range == 0) range = 1.0;
        return 10 + (w - 20) * (x - m_minX) / range;
    };

    p.setPen(QPen(Qt::gray, 1));
    if (m_minX <= 0 && m_maxX >= 0) {
        int zeroX = toScreen(0.0);
        p.drawLine(zeroX, y - 10, zeroX, y + 10);
        p.drawText(zeroX - 10, y + 25, "0");
    }

    int px = toScreen(m_currentPos);
    p.setBrush(Qt::red);
    p.setPen(Qt::NoPen);
    p.drawEllipse(QPoint(px, y), 8, 8);

    p.setPen(Qt::black);
    p.drawText(px - 15, y - 15, QString::number(m_currentPos, 'f', 2));
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Random Walk Simulator (Task 2)");
    resize(800, 600);
    setupUi();

    m_animation = new QVariantAnimation(this);
    m_animation->setDuration(1000);

    connect(m_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant& val){
        m_walkView->setPointPosition(val.toDouble());
    });
    connect(m_animation, &QVariantAnimation::finished, this, &MainWindow::doNextStep);
}

void MainWindow::setupUi() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    QHBoxLayout* controlLayout = new QHBoxLayout();

    controlLayout->addWidget(new QLabel("Initial Pos:"));
    m_initPosSpin = new QDoubleSpinBox();
    m_initPosSpin->setRange(-10000, 10000);
    controlLayout->addWidget(m_initPosSpin);

    controlLayout->addWidget(new QLabel("Steps (n):"));
    m_stepsSpin = new QSpinBox();
    m_stepsSpin->setRange(1, 1000);
    m_stepsSpin->setValue(10);
    controlLayout->addWidget(m_stepsSpin);

    QPushButton* btnLoad = new QPushButton("Load Law");
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::loadLawFromFile);
    controlLayout->addWidget(btnLoad);

    m_lawStatusLabel = new QLabel("Law: Not loaded");
    m_lawStatusLabel->setStyleSheet("color: red;");
    controlLayout->addWidget(m_lawStatusLabel);

    controlLayout->addStretch();

    m_btnStart = new QPushButton("Start");
    m_btnStop = new QPushButton("Stop");
    m_btnStop->setEnabled(false);

    connect(m_btnStart, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(m_btnStop, &QPushButton::clicked, this, &MainWindow::stopSimulation);

    controlLayout->addWidget(m_btnStart);
    controlLayout->addWidget(m_btnStop);
    mainLayout->addLayout(controlLayout);

    m_walkView = new WalkView();
    mainLayout->addWidget(m_walkView, 1);

    mainLayout->addWidget(new QLabel("<b>Theoretical Final Distribution (after n steps):</b>"));
    m_resultsTable = new QTableWidget(0, 2);
    m_resultsTable->setHorizontalHeaderLabels({"Final Position (X)", "Probability (P)"});
    m_resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(m_resultsTable, 2);
}

void MainWindow::loadLawFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Law of Movement", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty()) {
        try {
            m_stepLaw.loadFromFile(fileName.toStdString());
            m_lawStatusLabel->setText("Law: Loaded");
            m_lawStatusLabel->setStyleSheet("color: green;");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}

void MainWindow::calculateAndDisplayFinalDistribution() {
    DiscreteRandomVariable finalDist(std::map<double, double>{{m_initPosSpin->value(), 1.0}});

    int n = m_stepsSpin->value();
    for (int i = 0; i < n; ++i) {
        finalDist = finalDist + m_stepLaw;
    }

    m_walkView->setRange(finalDist.getMinValue(), finalDist.getMaxValue());

    const auto& dist = finalDist.getDistribution();
    m_resultsTable->setRowCount(0);
    for (const auto& [val, prob] : dist) {
        int row = m_resultsTable->rowCount();
        m_resultsTable->insertRow(row);
        m_resultsTable->setItem(row, 0, new QTableWidgetItem(QString::number(val, 'f', 2)));
        m_resultsTable->setItem(row, 1, new QTableWidgetItem(QString::number(prob, 'f', 6)));
    }
}

void MainWindow::startSimulation() {
    if (m_stepLaw.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please load a law of movement first.");
        return;
    }

    m_isSimulating = true;
    m_btnStart->setEnabled(false);
    m_btnStop->setEnabled(true);
    m_initPosSpin->setEnabled(false);
    m_stepsSpin->setEnabled(false);

    m_currentStep = 0;
    m_totalSteps = m_stepsSpin->value();
    m_currentX = m_initPosSpin->value();

    calculateAndDisplayFinalDistribution();

    m_walkView->setPointPosition(m_currentX);
    doNextStep();
}

void MainWindow::stopSimulation() {
    m_isSimulating = false;
    m_animation->stop();

    m_btnStart->setEnabled(true);
    m_btnStop->setEnabled(false);
    m_initPosSpin->setEnabled(true);
    m_stepsSpin->setEnabled(true);
}

void MainWindow::doNextStep() {
    if (!m_isSimulating) return;

    if (m_currentStep >= m_totalSteps) {
        stopSimulation();
        return;
    }

    double stepVal = m_stepLaw.generateRandomValue();
    double nextX = m_currentX + stepVal;

    m_animation->setStartValue(m_currentX);
    m_animation->setEndValue(nextX);
    m_animation->start();

    m_currentX = nextX;
    m_currentStep++;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}