#include "window.h"

#include <QApplication>
#include <QPainter>
#include <QPen>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    window w;
    w.show();
    return a.exec();
}
