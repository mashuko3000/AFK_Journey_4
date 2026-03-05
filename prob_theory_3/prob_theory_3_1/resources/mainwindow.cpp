//
// Created by mvideo on 01/03/26.
//

#include "mainwindow.hpp"
#include "mainwindow.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    setup_ui();

    m_workerThread = new QThread(this);
    m_worker = new simulator_worker();
    m_worker->moveToThread(m_workerThread);

    setup_connections();
    m_workerThread->start();
}

MainWindow::~MainWindow() {
    m_workerThread->quit();
    m_workerThread->wait();
    delete m_worker;

void MainWindow::setup_ui() {
    auto* centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    setCentralWidget(centralWidget);

    auto* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(20);

    m_inputPanel = new input_panel(this);
    m_visualPanel = new visual_panel(this);
    m_statsPanel = new statistics_panel(this);

    m_inputPanel->setFixedWidth(380);
    mainLayout->addWidget(m_inputPanel);

    auto* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(20);
    rightLayout->addWidget(m_visualPanel, 6);
    rightLayout->addWidget(m_statsPanel, 4);
    mainLayout->addLayout(rightLayout);

    this->setStyleSheet(R"(
        * {
            font-family: 'JetBrains Mono';
            color: #7D5A5A;
        }

        QMainWindow, QWidget#centralWidget { background-color: #FFF5F7; }

        QLabel#panelHeader, QLabel#titleLabel, QLabel#headerLabel {
            font-family: 'Ubuntu', sans-serif;
            font-size: 22px;
            font-weight: 900;
            color: #FF85A2;
            text-transform: uppercase;
            letter-spacing: 1.5px;
            margin-bottom: 10px;
        }

        input_panel, visual_panel, statistics_panel {
            background-color: #FFFFFF;
            border-radius: 30px;
            padding: 10px;
        }

        QTableWidget {
            background-color: white;
            border: none;
            gridline-color: #FFF5F7;
            outline: 0;
        }
        QHeaderView::section {
            background-color: #FFF5F7;
            color: #FF85A2;
            padding: 8px;
            border: none;
            font-weight: bold;
            font-size: 12px;
        }
        QTableCornerButton::section { background-color: #FFF5F7; border: none; }

        QSpinBox::up-button, QSpinBox::down-button {
            width: 0px;
            height: 0px;
            border: none;
            background: transparent;
        }
        QSpinBox {
            background-color: #FFF5F7;
            border: 2px solid #FFD1DC;
            border-radius: 12px;
            padding: 8px;
            font-size: 14px;
        }

        QSlider::groove:horizontal {
            height: 6px;
            background: #FFE5EC;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            background: #FF85A2;
            border: 2px solid white;
            width: 16px;
            height: 16px;
            margin: -5px 0;
            border-radius: 8px;
        }

        QProgressBar {
            background-color: #FFF5F7;
            border-radius: 12px;
            text-align: center;
            color: #FF85A2;
            font-weight: 800;
            border: 1px solid #FFE5EC;
            height: 22px;
        }
        QProgressBar::chunk {
            background-color: #FFC2D1;
            border-radius: 11px;
        }

        QLabel#statValue {
            color: #FF85A2;
            font-size: 18px;
            font-weight: 900;
        }

        QTextEdit {
            background-color: #FFFBFC;
            border: 2px solid #FFF5F7;
            border-radius: 20px;
            padding: 15px;
            font-size: 18px;
            font-weight: 600;
            color: #FF85A2;
        }

        QPushButton {
            background-color: #FFC2D1;
            color: white;
            border-radius: 15px;
            padding: 12px;
            font-weight: 800;
            border: none;
        }

        QPushButton:disabled {
            background-color: #D0D0D0; /* Чистый светло-серый (Bento-стиль) */
            color: #8E8E8E;           /* Темно-серый текст */
            border: 1px solid #C0C0C0; /* Едва заметная рамка */
        }

        /* Стиль для полей ввода (чтобы тоже не синели) */
        QLineEdit:disabled, QSpinBox:disabled, QComboBox:disabled {
            background-color: #F5F5F5;
            color: #B0B0B0;
            border: 1px solid #E0E0E0;
        }
        QPushButton:hover { background-color: #FFADC2; }
        QPushButton:pressed { background-color: #FF85A2; }
    )");
}

void MainWindow::setup_connections() {
    connect(m_inputPanel, &input_panel::batch_step_requested, m_worker, &simulator_worker::run_batch_step);
    connect(m_inputPanel, &input_panel::start_requested, m_worker, &simulator_worker::run_simulation);
    connect(m_inputPanel, &input_panel::step_requested, m_worker, &simulator_worker::run_single_step);
    connect(m_inputPanel, &input_panel::stop_requested, m_worker, &simulator_worker::stop);
    connect(m_inputPanel, &input_panel::reset_requested, this, [this]() {
        m_worker->reset_stats();
        m_inputPanel->set_controls_enabled(true);
        m_statsPanel->set_progress(0);
    });
    connect(m_worker, &simulator_worker::stats_updated, m_statsPanel, &statistics_panel::update_stats); // Это обновит нули на панели
    connect(m_worker, &simulator_worker::progress_updated, m_statsPanel, &statistics_panel::set_progress);

    connect(m_worker, &simulator_worker::progress_updated, this, [this](int percent) {
        if (percent >= 100) {
            m_inputPanel->set_controls_enabled(false);
        }
    });
    connect(m_worker, &simulator_worker::stats_updated, m_statsPanel, &statistics_panel::update_stats);
    connect(m_worker, &simulator_worker::finished, [this](double a, double b, double c, double avg){
        // Обновляем статистику финальными данными
        m_statsPanel->update_stats(a, b, c, avg, m_worker->get_total_runs()); // Если добавишь геттер
    });
    connect(m_worker, &simulator_worker::iteration_ready, m_visualPanel, &visual_panel::display_iteration);
    connect(m_worker, &simulator_worker::error_occurred, this, [](const QString& msg){
        QMessageBox::critical(nullptr, "Simulation Error", msg);
    });

}
