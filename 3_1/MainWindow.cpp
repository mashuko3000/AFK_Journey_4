//
// Created by maria on 15.04.2026.
//

#include "MainWindow.hpp"
#include "MainWindow.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCharts/QValueAxis>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Discrete Random Variable Modeler");
    resize(1000, 600);
    setupUi();
    loadSampleData();
}

void MainWindow::setupUi() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    QVBoxLayout* leftLayout = new QVBoxLayout();

    m_table = new QTableWidget(0, 2);
    m_table->setHorizontalHeaderLabels({"Value (x)", "Probability (P)"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    leftLayout->addWidget(m_table);

    m_statsLabel = new QLabel("Statistics will appear here.");
    m_statsLabel->setWordWrap(true);
    leftLayout->addWidget(m_statsLabel);

    QPushButton* btnLoad = new QPushButton("Load from File");
    QPushButton* btnSave = new QPushButton("Save to File");
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadFromFile);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveToFile);

    leftLayout->addWidget(btnLoad);
    leftLayout->addWidget(btnSave);

    m_tabWidget = new QTabWidget();
    m_polylineView = new QChartView();
    m_polylineView->setRenderHint(QPainter::Antialiasing);
    m_cdfView = new QChartView();
    m_cdfView->setRenderHint(QPainter::Antialiasing);

    m_tabWidget->addTab(m_polylineView, "Polyline (Distribution Law)");
    m_tabWidget->addTab(m_cdfView, "CDF (Cumulative Function)");

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addWidget(m_tabWidget, 2);
}

void MainWindow::loadSampleData() {
    std::map<double, double> sample = {
            {1.0, 0.2}, {2.0, 0.3}, {3.0, 0.4}, {4.0, 0.1}
    };
    m_drv = DiscreteRandomVariable(sample);
    updateViews();
}

void MainWindow::onLoadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Distribution File", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty()) {
        try {
            m_drv.loadFromFile(fileName.toStdString());
            updateViews();
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}

void MainWindow::onSaveToFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Distribution File", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty()) {
        try {
            m_drv.saveToFile(fileName.toStdString());
            QMessageBox::information(this, "Success", "Saved successfully.");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}

void MainWindow::updateViews() {
    updateTable();
    updateStats();
    updatePolylineChart();
    updateCDFChart();
}

void MainWindow::updateTable() {
    const auto& dist = m_drv.getDistribution();
    m_table->setRowCount(0);
    for (const auto& [val, prob] : dist) {
        int row = m_table->rowCount();
        m_table->insertRow(row);
        m_table->setItem(row, 0, new QTableWidgetItem(QString::number(val)));
        m_table->setItem(row, 1, new QTableWidgetItem(QString::number(prob)));
    }
}

void MainWindow::updateStats() {
    QString text = QString("<b>Expected Value (E):</b> %1<br>"
                           "<b>Variance (D):</b> %2<br>"
                           "<b>Skewness:</b> %3<br>"
                           "<b>Kurtosis:</b> %4")
            .arg(m_drv.expectedValue())
            .arg(m_drv.variance())
            .arg(m_drv.skewness())
            .arg(m_drv.kurtosis());
    m_statsLabel->setText(text);
}

void MainWindow::updatePolylineChart() {
    QChart* chart = new QChart();
    QLineSeries* series = new QLineSeries();

    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();

    for (const auto& [val, prob] : m_drv.getDistribution()) {
        series->append(val, prob);
        if (val < minX) minX = val;
        if (val > maxX) maxX = val;
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Polyline of Probabilities");
    chart->legend()->hide();

    if (chart->axes(Qt::Horizontal).size() > 0) {
        auto axisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
        double margin = (maxX - minX) * 0.1;
        if (margin == 0) margin = 1.0;
        axisX->setRange(minX - margin, maxX + margin);
    }
    if (chart->axes(Qt::Vertical).size() > 0) {
        auto axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
        axisY->setRange(0.0, 1.0);
    }

    m_polylineView->setChart(chart);
}

void MainWindow::updateCDFChart() {
    QChart* chart = new QChart();
    QLineSeries* series = new QLineSeries();

    const auto& dist = m_drv.getDistribution();
    if (dist.empty()) return;

    double minX = dist.begin()->first;
    double maxX = dist.rbegin()->first;
    double margin = (maxX - minX) * 0.2;
    if (margin == 0) margin = 1.0;

    double currentProb = 0.0;

    series->append(minX - margin, currentProb);

    for (const auto& [val, prob] : dist) {
        series->append(val, currentProb);
        currentProb += prob;
        series->append(val, currentProb);
    }

    series->append(maxX + margin, currentProb);

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Cumulative Distribution Function F(x)");
    chart->legend()->hide();

    if (chart->axes(Qt::Horizontal).size() > 0) {
        auto axisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
        axisX->setRange(minX - margin, maxX + margin);
    }
    if (chart->axes(Qt::Vertical).size() > 0) {
        auto axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
        axisY->setRange(0.0, 1.1);
    }

    m_cdfView->setChart(chart);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}