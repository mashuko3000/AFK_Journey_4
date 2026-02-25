#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include "simulator.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void onRunClicked();
private:
    void setupUi();
    void setupStyles();

    QSlider *sliderN, *sliderR, *sliderK, *sliderG;
    QLabel *lblN, *lblR, *lblK, *lblG;
    QComboBox *comboMode;
    QPushButton *btnRun;

    QLabel *lblPA, *lblPB;
    QTableWidget *logTable;
};
#endif
