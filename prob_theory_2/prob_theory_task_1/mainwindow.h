#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QMap>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <QLabel>
#include<QWheelEvent>
#include "simulator.h"
#include <QtOpenGLWidgets/QOpenGLWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void onLoadFile();
    void onNextStep();
    void onReset();
    void applyFilter(int type);

private:
    void setupGraphLayout();
    void updateGraphColors();
    void updateStats();
    QString getStyleSheet();

    QGraphicsView *graphView;
    QGraphicsScene *scene;
    QTextEdit *resultsLog;
    QLabel *lblHealthy;
    QLabel *lblInfected;
    QLabel *lblRecovered;

    simulator sim;
    QMap<uint32_t, QGraphicsEllipseItem*> nodeItems;
};


#endif // MAINWINDOW_H
