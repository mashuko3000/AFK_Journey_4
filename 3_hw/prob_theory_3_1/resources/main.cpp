#include <QApplication>
#include <QPushButton>

#include "mainwindow.hpp"

int main(int argc, char *argv[]) {
    qputenv("QT_QPA_PLATFORM", "xcb");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}