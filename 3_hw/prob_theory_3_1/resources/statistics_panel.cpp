//
// Created by mvideo on 01/03/26.
//

#include "statistics_panel.hpp"

statistics_panel::statistics_panel(QWidget *parent) : QWidget(parent) {
    this->setAttribute(Qt::WA_StyledBackground, true);
    setup_ui();
}

void statistics_panel::setup_ui() {
    auto* mainLayout = new QVBoxLayout(this);

    auto* header = new QLabel("Simulation Statistics", this);
    header->setObjectName("statValue");
    mainLayout->addWidget(header);

    auto* progressGroup = new QGroupBox("Current Progress", this);
    auto* progressLayout = new QVBoxLayout(progressGroup);
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_iterLabel = new QLabel("Iterations: 0", this);
    progressLayout->addWidget(m_progressBar);
    progressLayout->addWidget(m_iterLabel);
    mainLayout->addWidget(progressGroup);

    auto* resultsGroup = new QGroupBox("Probability Metrics", this);
    auto* resultsLayout = new QGridLayout(resultsGroup);

    m_probALabel = create_value_label();
    m_probBLabel = create_value_label();
    m_probCLabel = create_value_label();
    m_avgConnLabel = create_value_label();

    resultsLayout->addWidget(new QLabel("P(A) - Full Connection:"), 0, 0);
    resultsLayout->addWidget(m_probALabel, 0, 1);

    resultsLayout->addWidget(new QLabel("P(B) - D-Connections:"), 1, 0);
    resultsLayout->addWidget(m_probBLabel, 1, 1);

    resultsLayout->addWidget(new QLabel("P(C) - Total Disconnect:"), 2, 0);
    resultsLayout->addWidget(m_probCLabel, 2, 1);

    resultsLayout->addWidget(new QLabel("Avg Connections per Chain:"), 3, 0);
    resultsLayout->addWidget(m_avgConnLabel, 3, 1);

    mainLayout->addWidget(resultsGroup);
    mainLayout->addStretch();
}

QLabel* statistics_panel::create_value_label() {
    auto* label = new QLabel("0.0000", this);
    label->setObjectName("statValue");
    label->setAlignment(Qt::AlignRight);
    return label;
}

void statistics_panel::update_stats(double probA, double probB, double probC, double avgConn, size_t currentIter) {
    m_probALabel->setText(QString::number(probA, 'f', 4));
    m_probBLabel->setText(QString::number(probB, 'f', 4));
    m_probCLabel->setText(QString::number(probC, 'f', 4));
    m_avgConnLabel->setText(QString::number(avgConn, 'f', 2));
    m_iterLabel->setText(QString("Iterations: %1").arg(currentIter));
}

void statistics_panel::set_progress(int percent) {
    m_progressBar->setValue(percent);
}

void statistics_panel::clear() {
    m_progressBar->setValue(0);
    m_probALabel->setText("0.0000");
    m_probBLabel->setText("0.0000");
    m_probCLabel->setText("0.0000");
    m_avgConnLabel->setText("0.00");
    m_iterLabel->setText("Iterations: 0");
}
