#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class window;
}
QT_END_NAMESPACE

class window : public QMainWindow
{
    Q_OBJECT

public:
    window(QWidget *parent = nullptr);
    ~window();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_calculateButton_clicked();

private:
    Ui::window *ui;
    std::vector<double> historyA;
    std::vector<double> historyB;
    bool simulated = false;
    void drawGraph(QPainter &p, const std::vector<double> &data, int ox, int oy, int w, int h);
};
#endif // WINDOW_H
