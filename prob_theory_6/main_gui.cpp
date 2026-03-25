#include <QApplication>
#include <QPushButton>
/*
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
*/
#include <QApplication>
#include "Simulation/MainWindow.hpp"
#include "Simulation/Simulation.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto sim = std::make_unique<Simulation>();
    MainWindow window(std::move(sim));
    window.show();

    return app.exec();
}