//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_1_MAINWINDOW_HPP
#define PROB_THEORY_3_1_MAINWINDOW_HPP


#include <QMainWindow>
#include <QThread>
#include "input_panel.hpp"
#include "visual_panel.hpp"
#include "statistics_panel.hpp"
#include "simulator_worker.hpp"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    input_panel* m_inputPanel;
    visual_panel* m_visualPanel;
    statistics_panel* m_statsPanel;

    QThread* m_workerThread;
    simulator_worker* m_worker;

    void setup_ui();
    void setup_connections();
};

#endif //PROB_THEORY_3_1_MAINWINDOW_HPP
