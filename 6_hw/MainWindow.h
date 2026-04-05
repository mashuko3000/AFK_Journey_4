//
// Created by maria on 05.04.2026.
//

#ifndef INC_6_HW_MAINWINDOW_H
#define INC_6_HW_MAINWINDOW_H

#include<QMainWindow>
#include <QTableWidget>
#include "Interactive.h"

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT
    QTableWidget *table;
    Interactive *canvas;
public:
    MainWindow();
private slots:
    void addRow();
    void removeRow();
    void syncData();

private:
    void addDropShadow(QWidget* widget);
    void applyStyle();
};


#endif //INC_6_HW_MAINWINDOW_H
