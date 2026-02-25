#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi();
    setupStyles();
    setWindowTitle("Rumor Simulation System");
    resize(1100, 750);
}

void MainWindow::setupUi() {
    auto *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    auto *topPanel = new QFrame();
    topPanel->setObjectName("bento");
    topPanel->setFixedHeight(90);
    auto *topLayout = new QHBoxLayout(topPanel);

    auto *titleLabel = new QLabel("RUMOR SIMULATION SYSTEM");
    titleLabel->setStyleSheet("font-size: 18px; color: #FF758F; letter-spacing: 1px;");

    lblPA = new QLabel("P(A): 0.0000");
    lblPB = new QLabel("P(B): 0.0000");
    lblPA->setObjectName("statLabel"); lblPB->setObjectName("statLabel");

    topLayout->addWidget(titleLabel);
    topLayout->addStretch();
    topLayout->addWidget(lblPA);
    topLayout->addSpacing(40);
    topLayout->addWidget(lblPB);
    mainLayout->addWidget(topPanel);

    auto *midLayout = new QHBoxLayout();

    auto *side = new QFrame();
    side->setObjectName("bento");
    side->setFixedWidth(320);
    auto *sLayout = new QVBoxLayout(side);

    auto addS = [&](QString t, int min, int max, int def, QLabel** l) {
        auto *c = new QWidget(); auto *v = new QVBoxLayout(c); auto *h = new QHBoxLayout();
        *l = new QLabel(QString::number(def));
        h->addWidget(new QLabel(t)); h->addStretch(); h->addWidget(*l);
        auto *sl = new QSlider(Qt::Horizontal);
        sl->setRange(min, max); sl->setValue(def);
        v->addLayout(h); v->addWidget(sl);
        sLayout->addWidget(c);
        connect(sl, &QSlider::valueChanged, [=](int val){ (*l)->setText(QString::number(val)); });
        return sl;
    };

    sLayout->addWidget(new QLabel("SIMULATION PARAMETERS"), 0, Qt::AlignCenter);
    sLayout->addSpacing(10);
    sliderN = addS("Total Population", 10, 2000, 100, &lblN);
    sliderR = addS("Maximum Steps", 1, 500, 50, &lblR);
    sliderK = addS("Number of Trials", 100, 50000, 1000, &lblK);
    sliderG = addS("Group Spread Size", 1, 50, 5, &lblG);

    comboMode = new QComboBox();
    comboMode->addItem("Single Person Mode", 0);
    comboMode->addItem("Group Diffusion Mode", 1);
    sLayout->addWidget(new QLabel("Diffusion Mode:"));
    sLayout->addWidget(comboMode);

    sLayout->addStretch();
    btnRun = new QPushButton("RUN SIMULATION 🚀");
    sLayout->addWidget(btnRun);
    midLayout->addWidget(side);

    logTable = new QTableWidget(0, 4);
    logTable->setObjectName("bentoTable");
    logTable->setHorizontalHeaderLabels({"Trial ID", "Steps", "Total Informed", "Exit Reason"});
    logTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    logTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    logTable->setCornerButtonEnabled(false);
    midLayout->addWidget(logTable, 5);

    mainLayout->addLayout(midLayout);
    setCentralWidget(central);

    connect(btnRun, &QPushButton::clicked, this, &MainWindow::onRunClicked);
}

void MainWindow::onRunClicked() {
    simulator_config_t config;
    config.params.population = sliderN->value();
    config.params.max_steps = sliderR->value();
    config.params.group_size = sliderG->value();
    config.params.mode = static_cast<spread_mode_t>(comboMode->currentIndex());
    config.num_trials = sliderK->value();
    config.keep_details = (config.num_trials <= 5000);

    rumor_simulator sim(config);
    sim.run();

    auto summary = sim.get_summary();
    lblPA->setText(QString("P(A): %1").arg(summary.p_A, 0, 'f', 4));
    lblPB->setText(QString("P(B): %1").arg(summary.p_B, 0, 'f', 4));

    logTable->setRowCount(0);
    if (config.keep_details) {
        auto details = sim.get_all_runs();
        int toShow = std::min((int)details.size(), 1000);
        logTable->setRowCount(toShow);

        for(int i = 0; i < toShow; ++i) {
            auto *it0 = new QTableWidgetItem(QString::number(i + 1));
            auto *it1 = new QTableWidgetItem(QString::number(details[i].steps_done));
            auto *it2 = new QTableWidgetItem(QString::number(details[i].unique_heard));
            auto *it3 = new QTableWidgetItem(QString::fromStdString(details[i].stop_reason));

            it0->setTextAlignment(Qt::AlignCenter);
            it1->setTextAlignment(Qt::AlignCenter);
            it2->setTextAlignment(Qt::AlignCenter);
            it3->setTextAlignment(Qt::AlignCenter);

            logTable->setItem(i, 0, it0);
            logTable->setItem(i, 1, it1);
            logTable->setItem(i, 2, it2);
            logTable->setItem(i, 3, it3);
        }
    }
}

void MainWindow::setupStyles() {
    this->setStyleSheet(R"(
        QMainWindow { background-color: #FDF2F4; }

        #bento {
            background-color: white;
            border: 2px solid #FADADD;
            border-radius: 20px;
            padding: 10px;
        }

        QLabel { color: #8E8E8E; font-weight: bold; font-family: 'Segoe UI'; }
        #statLabel { color: #FF758F; font-size: 20px; font-weight: 800; }

        QSlider::groove:horizontal { height: 6px; background: #FADADD; border-radius: 3px; }
        QSlider::handle:horizontal {
            background: #FF9AA2; width: 18px; height: 18px; margin: -6px 0; border-radius: 9px;
            border: 2px solid white;
        }

        QPushButton {
            background-color: #FF9AA2; color: white; border-radius: 12px;
            padding: 15px; font-weight: bold; border: none; font-size: 13px;
        }
        QPushButton:hover { background-color: #FFB7B2; }

        QComboBox {
            background: white; border: 2px solid #FADADD; border-radius: 8px;
            padding: 8px; color: #8E8E8E; font-weight: bold;
        }

QTableWidget#bentoTable {
            background-color: white;
            border: 2px solid #FADADD;
            border-radius: 20px;
            gridline-color: #FDF2F4;

            font-family: 'Segoe UI Mono', 'Consolas', monospace;
            font-size: 13px;
            font-weight: 600;
            color: #FF758F;
            outline: none;
        }

        QTableWidget QTableCornerButton::section {
            background-color: transparent;
            border: none;
        }

        QTableWidget::item {
            padding: 12px;
            border-bottom: 1px solid #FDF2F4;
        }

        QTableWidget::item:selected {
            background-color: #FADADD;
            color: white;
            border-radius: 0px;
        }

        QHeaderView::section {
            background-color: #FFB7B2;
            color: white;
            padding: 10px;
            border: none;
            font-weight: bold;
            font-family: 'Segoe UI';
            font-size: 11px;
            text-transform: uppercase;
        }
    )");
}
