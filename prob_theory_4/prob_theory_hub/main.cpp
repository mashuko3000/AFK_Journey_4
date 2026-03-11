#include <QApplication>
#include <QPushButton>

#include "base_ui/UniversalBaseWindow.hpp"
#include "drunk_man/DrunkardEngine.hpp"
#include "drunk_man/DrankManWindow.hpp"
#include"point/RandomWalking.hpp"
#include "point/HistoryLogger.hpp"
#include "point/Config.hpp"
#include <iostream>
#include "point/Window.hpp"
#include "rw_return_to_origin/PontWalking.hpp"
#include <fstream>
#include "complex_random_walking/ComplexWindow.hpp"

#include "stairs/StairsWindow.hpp"
#include "stairs/ConfigLoader.hpp"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //StairsWindow w;
    //Window w;
    DrankManWindow w;
    //ComplexWindow w;
    w.show();
    return a.exec();
    return 0;
}


