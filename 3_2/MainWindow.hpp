//
// Created by maria on 15.04.2026.
//

#ifndef INC_3_2_MAINWINDOW_HPP
#define INC_3_2_MAINWINDOW_HPP


#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QVariantAnimation>
#include "DiscreteRandomVariable.hpp"

// Кастомный виджет для отрисовки оси блуждания
class WalkView : public QWidget {
Q_OBJECT
public:
    explicit WalkView(QWidget* parent = nullptr);
    void setRange(double minX, double maxX);
    void setPointPosition(double x);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    double m_minX = -10.0;
    double m_maxX = 10.0;
    double m_currentPos = 0.0;
};

// Главное окно симулятора
class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void loadLawFromFile();
    void startSimulation();
    void stopSimulation();
    void doNextStep();

private:
    void setupUi();
    void calculateAndDisplayFinalDistribution();

    DiscreteRandomVariable m_stepLaw;

    QDoubleSpinBox* m_initPosSpin;
    QSpinBox* m_stepsSpin;
    QLabel* m_lawStatusLabel;
    QPushButton* m_btnStart;
    QPushButton* m_btnStop;

    WalkView* m_walkView;
    QTableWidget* m_resultsTable;

    QVariantAnimation* m_animation;

    // Состояние моделирования
    bool m_isSimulating = false;
    int m_currentStep = 0;
    int m_totalSteps = 0;
    double m_currentX = 0.0;
};


#endif //INC_3_2_MAINWINDOW_HPP
