//
// Created by mvideo on 25/03/26.
//

#ifndef PROB_THEORY_6_MAINWINDOW_HPP
#define PROB_THEORY_6_MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <memory>
#include<QScrollArea>
#include "Widget.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(std::unique_ptr<ISimulation> sim, QWidget* parent = nullptr);

private slots:
    void runSimulation();

private:
    std::unique_ptr<ISimulation> m_simulation;
    CWidget* m_canvas;
    QScrollArea* m_scrollArea;

    QDoubleSpinBox* m_dInput;
    QDoubleSpinBox* m_LInput;
    QSpinBox* m_countInput;

    QLabel* m_empiricalLabel;
    QLabel* m_exactLabel;
};


#endif //PROB_THEORY_6_MAINWINDOW_HPP
