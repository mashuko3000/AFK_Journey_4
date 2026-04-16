//
// Created by maria on 15.04.2026.
//

#ifndef INC_3_1_MAINWINDOW_HPP
#define INC_3_1_MAINWINDOW_HPP


#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QTabWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "DiscreteRandomVariable.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void loadSampleData();
    void onLoadFromFile();
    void onSaveToFile();

private:
    void setupUi();
    void updateViews();
    void updateTable();
    void updateStats();
    void updatePolylineChart();
    void updateCDFChart();

    DiscreteRandomVariable m_drv;

    QTableWidget* m_table;
    QLabel* m_statsLabel;
    QTabWidget* m_tabWidget;
    QChartView* m_polylineView;
    QChartView* m_cdfView;
};


#endif //INC_3_1_MAINWINDOW_HPP
