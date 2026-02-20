#include "window.h"
#include "./ui_window.h"
#include <QPainter>
#include <QPen>

window::window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::window)
{
    ui->setupUi(this);
    srand(time(NULL));

    ui->spinBox->setMaximum(1000000);
    ui->spinBox->setValue(10000);

    this->setStyleSheet("background-color: #0f172a; color: white;");
}

window::~window()
{
    delete ui;
}

void window::on_calculateButton_clicked()
{
    int trials = ui->spinBox->value();
    historyA.clear();
    historyB.clear();

    int success_not_more_than_three = 0;
    int success_at_least_two = 0;

    double p[] = {0.6, 0.7, 0.8, 0.9};

    int step = std::max(1, trials / 500);

    for(int i = 0; i < trials; ++i)
    {
        int success = 0;

        for(int j = 0; j < 4; ++j)
        {
            double r = (double)rand() / RAND_MAX;
            if (r < p[j])
            {
                success++;
            }
        }

        if (success <= 3) success_not_more_than_three++;
        if (success >= 2) success_at_least_two++;

        if (i % step == 0)
        {
            historyA.push_back((double)success_not_more_than_three / (i + 1));
            historyB.push_back((double)success_at_least_two / (i + 1));
        }
    }
    double resA = (double)success_not_more_than_three / trials;
    double resB = (double)success_at_least_two / trials;

    ui->labelResultA->setText("P(A) not more 3: " + QString::number(resA, 'f', 4));
    ui->labelResultB->setText("P(B) at  least 2: " + QString::number(resB, 'f', 4));

    simulated = true;
    update();
}

void window::paintEvent(QPaintEvent *event)
{
    if (!simulated || historyA.empty()) return;

    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);

    int offsetX = 60;
    int offsetY = height() - 60;
    int w = width() - 120;
    int h = height() / 2;

    //painter.fillRect(offsetX, offsetY - h, w, h, QColor(15, 23, 42, 100));
    painter.setPen(QColor(50, 50, 80));
    painter.drawRect(offsetX, offsetY - h, w, h);

    QPen penA(QColor(34, 211, 238), 2);
    painter.setPen(penA);
    drawGraph(painter, historyA, offsetX, offsetY, w, h);

    painter.setPen(QPen(QColor(34, 211, 238), 1, Qt::DashLine));
    int targetYA = offsetY - (0.6976 * h);
    painter.drawLine(offsetX, targetYA, offsetX + w, targetYA);

    QPen penB(QColor(163, 230, 53), 2);
    painter.setPen(penB);
    drawGraph(painter, historyB, offsetX, offsetY, w, h);

    painter.setPen(QPen(QColor(163, 230, 53), 1, Qt::DashLine));
    int targetYB = offsetY - (0.9504 * h);
    painter.drawLine(offsetX, targetYB, offsetX + w, targetYB);
}


void window::drawGraph(QPainter &p, const std::vector<double> &data, int ox, int oy, int w, int h) {
    for (size_t i = 1; i < data.size(); ++i) {
        int x1 = ox + ((i - 1) * w) / data.size();
        int y1 = oy - (data[i - 1] * h);
        int x2 = ox + (i * w) / data.size();
        int y2 = oy - (data[i] * h);
        p.drawLine(x1, y1, x2, y2);
    }
}
