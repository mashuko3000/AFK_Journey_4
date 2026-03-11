//
// Created by mvideo on 01/03/26.
//

#include "input_panel.hpp"
#include <QHeaderView>
#include <QRandomGenerator>

input_panel::input_panel(QWidget *parent) : QWidget(parent)
{
    setup_ui();
    setup_connections();
}

void input_panel::setup_ui() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    m_titleLabel = new QLabel("Cluster Generation", this);
    m_titleLabel->setObjectName("titleLabel");
    mainLayout->addWidget(m_titleLabel, 0, Qt::AlignLeft);

    m_alphaGroup = new QGroupBox("Alphabet Configuration", this);
    auto* alphaLayout = new QVBoxLayout(m_alphaGroup);

    m_distTypeCombo = new QComboBox(this);
    m_distTypeCombo->addItem("Uniform Distribution", static_cast<int>(distribution_type::uniform));
    m_distTypeCombo->addItem("Discrete Distribution", static_cast<int>(distribution_type::discrete));

    m_alphabetEdit = new QLineEdit(this);
    m_alphabetEdit->setPlaceholderText("Enter alphabet symbols...");
    m_alphabetEdit->setText("01");

    m_weightsTable = new QTableWidget(0, 2, this);
    m_weightsTable->setHorizontalHeaderLabels({"Symbol", "Weight"});
    m_weightsTable->horizontalHeader()->setStretchLastSection(true);

    m_weightsTable->setMinimumHeight(120);
    m_weightsTable->setMaximumHeight(200);
    //m_weightsTable->setFixedHeight(150);

    m_randomizeWeightsBtn = new QPushButton("Randomize Weights", this);

    alphaLayout->addWidget(new QLabel("Distribution Type:"));
    alphaLayout->addWidget(m_distTypeCombo);
    alphaLayout->addWidget(new QLabel("Alphabet:"));
    alphaLayout->addWidget(m_alphabetEdit);
    alphaLayout->addWidget(m_weightsTable);
    alphaLayout->addWidget(m_randomizeWeightsBtn);
    mainLayout->addWidget(m_alphaGroup);

    m_paramsGroup = new QGroupBox("Simulation Parameters", this);
    auto* paramsLayout = new QGridLayout(m_paramsGroup);

    m_mSpin = new QSpinBox(this);
    m_mSpin->setRange(1, 100);
    m_mSpin->setValue(5);
    m_mSlider = new QSlider(Qt::Horizontal, this);
    m_mSlider->setRange(1, 100);
    m_mSlider->setValue(5);

    m_nSpin = new QSpinBox(this);
    m_nSpin->setRange(1, 50);
    m_nSpin->setValue(10);
    m_nSlider = new QSlider(Qt::Horizontal, this);
    m_nSlider->setRange(1, 50);
    m_nSlider->setValue(10);

    m_dSpin = new QSpinBox(this);
    m_dSpin->setRange(0, 20);
    m_dSpin->setValue(2);
    m_dSlider = new QSlider(Qt::Horizontal, this);
    m_dSlider->setRange(0, 20);
    m_dSlider->setValue(2);

    m_patternEdit = new QLineEdit(this);
    m_patternEdit->setPlaceholderText("e.g. 11");
    m_patternEdit->setText("11");

    paramsLayout->addWidget(new QLabel("M (Clusters):"), 0, 0);
    paramsLayout->addWidget(m_mSpin, 0, 1);
    paramsLayout->addWidget(m_mSlider, 0, 2);

    paramsLayout->addWidget(new QLabel("n (Length):"), 1, 0);
    paramsLayout->addWidget(m_nSpin, 1, 1);
    paramsLayout->addWidget(m_nSlider, 1, 2);

    paramsLayout->addWidget(new QLabel("d (Max Dist):"), 2, 0);
    paramsLayout->addWidget(m_dSpin, 2, 1);
    paramsLayout->addWidget(m_dSlider, 2, 2);

    paramsLayout->addWidget(new QLabel("Target Pattern:"), 3, 0);
    paramsLayout->addWidget(m_patternEdit, 3, 1, 1, 2);

    mainLayout->addWidget(m_paramsGroup);

    m_controlGroup = new QGroupBox("Execution Control", this);
    auto* controlLayout = new QVBoxLayout(m_controlGroup);

    m_iterSpin = new QSpinBox(this);
    m_iterSpin->setRange(1, 1000000);
    m_iterSpin->setValue(1000);
    m_iterSpin->setSingleStep(100);

    m_startBtn = new QPushButton("▶ Start Full Simulation", this);
    m_stopBtn = new QPushButton("⏹ Stop", this);
    m_stepBtn = new QPushButton("Step", this);
    m_resetBtn = new QPushButton("Reset Statistics", this);

    m_startBtn->setObjectName("runButton");
    m_stopBtn->setObjectName("stopButton");
    m_resetBtn->setObjectName("resetButton");
    m_stepBtn->setObjectName("stepButton");

    auto* batchLayout = new QHBoxLayout();
    m_batchStepBtn = new QPushButton("Batch Step", this);
    m_batchSizeSpin = new QSpinBox(this);
    m_batchSizeSpin->setRange(2, 5000);
    m_batchSizeSpin->setValue(100);
    batchLayout->addWidget(m_batchStepBtn, 2);
    batchLayout->addWidget(m_batchSizeSpin, 1);

    controlLayout->addWidget(new QLabel("Total Iterations:"));
    controlLayout->addWidget(m_iterSpin);
    controlLayout->addWidget(m_startBtn);
    controlLayout->addWidget(m_stopBtn);
    controlLayout->addWidget(m_stepBtn);
    controlLayout->addLayout(batchLayout);
    controlLayout->addWidget(m_resetBtn);

    mainLayout->addWidget(m_controlGroup);
    mainLayout->addStretch();

    on_alphabet_changed(m_alphabetEdit->text());
    on_dist_type_changed(m_distTypeCombo->currentIndex());
}

void input_panel::add_labeled_slider(QGridLayout* layout, const QString& label, QSpinBox* spin, QSlider* slider, int row) {
    layout->addWidget(new QLabel(label), row, 0);
    layout->addWidget(spin, row, 1);
    layout->addWidget(slider, row, 2);
}

void input_panel::setup_connections() {
    connect(m_mSpin, QOverload<int>::of(&QSpinBox::valueChanged), m_mSlider, &QSlider::setValue);
    connect(m_mSlider, &QSlider::valueChanged, m_mSpin, &QSpinBox::setValue);

    connect(m_nSpin, QOverload<int>::of(&QSpinBox::valueChanged), m_nSlider, &QSlider::setValue);
    connect(m_nSlider, &QSlider::valueChanged, m_nSpin, &QSpinBox::setValue);

    connect(m_dSpin, QOverload<int>::of(&QSpinBox::valueChanged), m_dSlider, &QSlider::setValue);
    connect(m_dSlider, &QSlider::valueChanged, m_dSpin, &QSpinBox::setValue);

    connect(m_alphabetEdit, &QLineEdit::textChanged, this, &input_panel::on_alphabet_changed);
    connect(m_distTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &input_panel::on_dist_type_changed);
    connect(m_randomizeWeightsBtn, &QPushButton::clicked, this, &input_panel::generate_random_weights);

    connect(m_batchStepBtn, &QPushButton::clicked, [this]() {
        emit batch_step_requested(get_config(), m_batchSizeSpin->value());
    });
    connect(m_startBtn, &QPushButton::clicked, [this]() { emit start_requested(get_config()); });
    connect(m_stepBtn, &QPushButton::clicked, [this]() { emit step_requested(get_config()); });
    connect(m_stopBtn, &QPushButton::clicked, this, &input_panel::stop_requested);
    connect(m_resetBtn, &QPushButton::clicked, this, &input_panel::reset_requested);
}

void input_panel::on_alphabet_changed(const QString& new_alpha) {
    m_weightsTable->setRowCount(new_alpha.length());
    for(int i = 0; i < new_alpha.length(); ++i) {
        m_weightsTable->setItem(i, 0, new QTableWidgetItem(QString(new_alpha[i])));
        if(!m_weightsTable->item(i, 1)) {
            m_weightsTable->setItem(i, 1, new QTableWidgetItem("1.0"));
        }
    }
}

void input_panel::on_dist_type_changed(int index) {
    bool isDiscrete = (m_distTypeCombo->itemData(index).toInt() == static_cast<int>(distribution_type::discrete));
    m_weightsTable->setEnabled(isDiscrete);
    m_randomizeWeightsBtn->setEnabled(isDiscrete);
}

void input_panel::generate_random_weights() {
    int rows = m_weightsTable->rowCount();
    if (rows == 0) return;

    std::vector<double> rands(rows);
    double sum = 0;
    for(int i = 0; i < rows; ++i) {
        rands[i] = QRandomGenerator::global()->generateDouble();
        sum += rands[i];
    }
    for(int i = 0; i < rows; ++i) {
        m_weightsTable->item(i, 1)->setText(QString::number(rands[i] / sum, 'f', 3));
    }
}

simulation_config input_panel::get_config() const {
    simulation_config config;
    config.alphabet = m_alphabetEdit->text();
    config.dist_type = static_cast<distribution_type>(m_distTypeCombo->currentData().toInt());
    config.pattern = m_patternEdit->text();
    config.M = m_mSpin->value();
    config.n = m_nSpin->value();
    config.d = m_dSpin->value();
    config.total_iterations = m_iterSpin->value();

    if(config.dist_type == distribution_type::discrete) {
        for(int i = 0; i < m_weightsTable->rowCount(); ++i) {
            config.weights.push_back(m_weightsTable->item(i, 1)->text().toDouble());
        }
    }
    return config;
}

void input_panel::set_controls_enabled(bool enabled) {
    m_startBtn->setEnabled(enabled);
    m_stepBtn->setEnabled(enabled);
    m_batchStepBtn->setEnabled(enabled);
}