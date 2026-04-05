//
// Created by maria on 05.04.2026.
//

#include "MainWindow.h"

#include<QHBoxLayout>
#include<QFrame>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>

MainWindow::MainWindow()
{
    setStyleSheet("QMainWindow {background-color: #F4F6F9;}");

    QWidget* central = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    QFrame* controlsCard = new QFrame();
    controlsCard->setObjectName("bentoCard");
    QVBoxLayout* controlsLayout = new QVBoxLayout(controlsCard);
    controlsLayout->setContentsMargins(20, 20, 20, 20);
    controlsLayout->setSpacing(15);
    addDropShadow(controlsCard);

    QLabel* header = new QLabel("Monomial Generators\\nI = ⟨x^a * y^b⟩");
    QFont f("Segoe UI", 12, QFont::Bold);
    header->setFont(f);
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("color: #3D405B; margin-bottom: 10px;");

    table = new QTableWidget(0, 2);
    table->setHorizontalHeaderLabels({"Power of X (a)", "Power of Y (b)"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->setShowGrid(false);

    QPushButton *btnAdd = new QPushButton("Add Generator");
    btnAdd->setObjectName("btnAdd");

    QPushButton *btnDel = new QPushButton("Remove Selected");
    btnDel->setObjectName("btnDel");

    controlsLayout->addWidget(header);
    controlsLayout->addWidget(table);
    controlsLayout->addWidget(btnAdd);
    controlsLayout->addWidget(btnDel);
    controlsLayout->addStretch();

    QFrame *canvasCard = new QFrame();
    canvasCard->setObjectName("bentoCard");
    QVBoxLayout *canvasLayout = new QVBoxLayout(canvasCard);
    canvasLayout->setContentsMargins(10, 10, 10, 10);
    addDropShadow(canvasCard);

    canvas = new Interactive();
    canvasLayout->addWidget(canvas);

    mainLayout->addWidget(controlsCard, 1);
    mainLayout->addWidget(canvasCard, 3);

    setCentralWidget(central);
    setWindowTitle("Ideal Staircase Visualizer - Bento Edition");
    resize(1100, 700);

    applyStyle();

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addRow);
    connect(btnDel, &QPushButton::clicked, this, &MainWindow::removeRow);
    connect(table, &QTableWidget::itemChanged, this, &MainWindow::syncData);

    addRow();
}
void MainWindow::addRow()
{
    table->blockSignals(true);
    int r = table->rowCount();
    table->insertRow(r);
    table->setItem(r, 0, new QTableWidgetItem("2"));
    table->setItem(r, 1, new QTableWidgetItem("3"));
    table->blockSignals(false);
    syncData();
}
void MainWindow::removeRow()
{
    table->removeRow(table->currentRow());
    syncData();
}
void MainWindow::syncData()
{
    QVector<Generator> gens;
    for (int i = 0; i < table->rowCount(); ++i)
    {
        auto itemA = table->item(i, 0);
        auto itemB = table->item(i, 1);
        if (itemA && itemB) {
            gens.push_back({itemA->text().toInt(), itemB->text().toInt()});
        }
    }
    canvas->setGenerators(gens);
}
void MainWindow::addDropShadow(QWidget* widget)
{
    auto* shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 15));
    shadow->setOffset(0, 6);
    widget->setGraphicsEffect(shadow);
}
void MainWindow::applyStyle()
{
    QString style = R"(
            QFrame#bentoCard {
                background-color: #FFFFFF;
                border-radius: 16px;
            }

            QTableWidget {
                background-color: #F8F9FA;
                border: 1px solid #E9ECEF;
                border-radius: 10px;
                color: #3D405B;
                selection-background-color: #E2E8F0;
                selection-color: #3D405B;
                font-family: "Segoe UI";
                font-size: 13px;
            }

            QTableWidget::item {
                padding: 5px;
                border-bottom: 1px solid #F1F3F5;
            }

            QHeaderView::section {
                background-color: #FFFFFF;
                color: #8C919B;
                font-weight: bold;
                border: none;
                border-bottom: 2px solid #E9ECEF;
                padding: 8px;
            }

            QPushButton {
                font-family: "Segoe UI";
                font-size: 13px;
                font-weight: bold;
                color: white;
                border-radius: 8px;
                padding: 10px 15px;
                border: none;
            }

            QPushButton#btnAdd {
                background-color: #A3C4BC; /* Muted Sage */
            }
            QPushButton#btnAdd:hover {
                background-color: #8EB3AA;
            }

            QPushButton#btnDel {
                background-color: #E29578; /* Muted Coral */
                margin-top: 5px;
            }
            QPushButton#btnDel:hover {
                background-color: #D68466;
            }
        )";
    this->setStyleSheet(this->styleSheet() + style);
}