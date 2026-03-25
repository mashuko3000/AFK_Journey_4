//
// Created by mvideo on 25/03/26.
//

#include "MainWindow.hpp"
#include <QString>

MainWindow::MainWindow(std::unique_ptr<ISimulation> sim, QWidget *parent)
        : QMainWindow(parent), m_simulation(std::move(sim))
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout* controlsLayout = new QHBoxLayout();

    m_dInput = new QDoubleSpinBox();
    m_dInput->setValue(50.0);
    m_dInput->setRange(1.0, 200.0);
    controlsLayout->addWidget(new QLabel("d (Distance):"));
    controlsLayout->addWidget(m_dInput);

    m_LInput = new QDoubleSpinBox();
    m_LInput->setValue(30.0);
    m_LInput->setRange(1.0, 200.0);
    controlsLayout->addWidget(new QLabel("L (Needle length):"));
    controlsLayout->addWidget(m_LInput);

    m_countInput = new QSpinBox();
    m_countInput->setRange(1, 100000);
    m_countInput->setValue(1000);
    controlsLayout->addWidget(new QLabel("Toss count:"));
    controlsLayout->addWidget(m_countInput);

    QPushButton* btnRun = new QPushButton("Toss Needles");
    connect(btnRun, &QPushButton::clicked, this, &MainWindow::runSimulation);
    controlsLayout->addWidget(btnRun);

    controlsLayout->addStretch();
    mainLayout->addLayout(controlsLayout, 0);

    m_canvas = new CWidget();
    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidget(m_canvas);
    m_scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(m_scrollArea, 1);

    QHBoxLayout* resultsLayout = new QHBoxLayout();
    m_empiricalLabel = new QLabel("Empirical probability: 0.0000");
    m_exactLabel = new QLabel("Exact probability: 0.0000");

    QFont resultFont = m_empiricalLabel->font();
    resultFont.setPointSize(12);
    resultFont.setBold(true);
    m_empiricalLabel->setFont(resultFont);
    m_exactLabel->setFont(resultFont);

    resultsLayout->addWidget(m_empiricalLabel);
    resultsLayout->addSpacing(30);
    resultsLayout->addWidget(m_exactLabel);
    resultsLayout->addStretch();

    mainLayout->addLayout(resultsLayout, 0);

    setCentralWidget(centralWidget);
    setWindowTitle("Simulation: Buffon's Needle");
    resize(1000, 700);
}

void MainWindow::runSimulation()
{
    double d = m_dInput->value();
    double L = m_LInput->value();
    int count = m_countInput->value();

    if (L > d) {
        m_LInput->setValue(d);
        L = d;
    }

    m_simulation->run(count, d, L);

    m_empiricalLabel->setText(QString("Empirical probability: %1").arg(m_simulation->getEmpiricalProb(), 0, 'f', 4));
    m_exactLabel->setText(QString("Exact probability: %1").arg(m_simulation->getTheoryProb(d, L), 0, 'f', 4));

    m_canvas->updateData(m_simulation->getResult(), d, L);
}
