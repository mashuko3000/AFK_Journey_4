#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <memory>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "../core/simulation.hpp"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleStartSimulation();
    void handleSingleStep();
    void handleReset();
    void updateStatsDisplay();

private:
    void setupUi();
    void createNewTree();
    void drawTree();
    void highlightPath(const iteration_data& data);

    std::unique_ptr<tree_model> m_tree;
    std::unique_ptr<simulation> m_sim;
    iteration_data m_current_path;

    QSpinBox* m_mSpin;
    QSpinBox* m_hSpin;
    QDoubleSpinBox* m_pStaySpin;
    QComboBox* m_distCombo;
    QDoubleSpinBox* m_distPParam;
    QLabel* m_pLabel;

    QGraphicsView* m_view;
    QGraphicsScene* m_scene;
    QTextEdit* m_statsText;
};

#endif // MAINWINDOW_HPP