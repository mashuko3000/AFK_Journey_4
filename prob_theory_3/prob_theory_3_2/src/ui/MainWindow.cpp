#include "MainWindow.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <functional>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi();
    createNewTree();
}

void MainWindow::setupUi() {
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto *mainLayout = new QHBoxLayout(centralWidget);

    auto *leftPanel = new QVBoxLayout();
    auto *settingsGroup = new QGroupBox("Параметры");
    auto *settingsLayout = new QVBoxLayout(settingsGroup);

    m_mSpin = new QSpinBox(); m_mSpin->setRange(2, 10); m_mSpin->setValue(2);
    m_hSpin = new QSpinBox(); m_hSpin->setRange(1, 10); m_hSpin->setValue(3);
    m_pStaySpin = new QDoubleSpinBox(); m_pStaySpin->setRange(0, 1); m_pStaySpin->setSingleStep(0.05);

    m_distCombo = new QComboBox();
    m_distCombo->addItems({"Равномерное", "Бернулли", "Биномиальное", "Геометрическое"});

    m_distPParam = new QDoubleSpinBox();
    m_distPParam->setRange(0.01, 0.99);
    m_distPParam->setValue(0.5);

    settingsLayout->addWidget(new QLabel("M (Ветки):"));
    settingsLayout->addWidget(m_mSpin);
    settingsLayout->addWidget(new QLabel("H (Высота):"));
    settingsLayout->addWidget(m_hSpin);
    settingsLayout->addWidget(new QLabel("P (Застревание):"));
    settingsLayout->addWidget(m_pStaySpin);
    settingsLayout->addWidget(new QLabel("Закон распределения:"));
    settingsLayout->addWidget(m_distCombo);
    settingsLayout->addWidget(new QLabel("P (для закона):"));
    settingsLayout->addWidget(m_distPParam);

    auto *btnStep = new QPushButton("Один шаг");
    auto *btnRun = new QPushButton("10 000 итераций");
    auto *btnReset = new QPushButton("Сбросить дерево");

    leftPanel->addWidget(settingsGroup);
    leftPanel->addWidget(btnStep);
    leftPanel->addWidget(btnRun);
    leftPanel->addWidget(btnReset);
    leftPanel->addStretch();

    auto *rightLayout = new QVBoxLayout();
    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene);
    m_view->setRenderHint(QPainter::Antialiasing);

    m_statsText = new QTextEdit();
    m_statsText->setReadOnly(true);
    m_statsText->setMaximumHeight(150);

    rightLayout->addWidget(new QLabel("Визуализация:"));
    rightLayout->addWidget(m_view, 1);
    rightLayout->addWidget(m_statsText);

    mainLayout->addLayout(leftPanel, 1);
    mainLayout->addLayout(rightLayout, 4);

    connect(btnRun, &QPushButton::clicked, this, &MainWindow::handleStartSimulation);
    connect(btnStep, &QPushButton::clicked, this, &MainWindow::handleSingleStep);
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::handleReset);
}

void MainWindow::createNewTree() {
    int m = m_mSpin->value();
    double p_dist = m_distPParam->value();
    i_distribution_model* dist = nullptr;

    switch (m_distCombo->currentIndex()) {
        case 0: dist = new uniform_distribution_model(m); break;
        case 1: dist = new bernoulli_distribution(p_dist, m); break;
        case 2: dist = new binomial_distribution(p_dist, m); break;
        case 3: dist = new geometric_distribution(p_dist, m); break;
    }

    if (dist) {
        m_tree = std::make_unique<tree_model>(m, m_hSpin->value(), dist, m_pStaySpin->value());
        m_sim = std::make_unique<simulation>(*m_tree);
        delete dist;
    }
    drawTree();
}

void MainWindow::drawTree() {
    m_scene->clear();
    if (!m_tree || !m_tree->get_root()) return;

    float w = 600.0f;
    float h = 400.0f;

    std::function<void(node*)> drawNodes = [&](node* n) {
        if (!n) return;
        float x = n->x * w;
        float y = n->y * h;

        for (auto* child : n->children) {
            m_scene->addLine(x, y, child->x * w, child->y * h, QPen(Qt::lightGray));
            drawNodes(child);
        }
        m_scene->addEllipse(x-4, y-4, 8, 8, QPen(Qt::black), QBrush(Qt::white))->setZValue(2);
    };

    drawNodes(m_tree->get_root());
    m_view->setSceneRect(-50, -50, w + 100, h + 100);
}

void MainWindow::highlightPath(const iteration_data& data) {
    float w = 600.0f;
    float h = 400.0f;

    for (size_t i = 0; i + 1 < data.path.size(); ++i) {
        node* curr = data.path[i];
        node* next = data.path[i+1];
        m_scene->addLine(curr->x * w, curr->y * h, next->x * w, next->y * h, QPen(Qt::red, 3))->setZValue(3);
    }

    if (data.end_node) {
        float ex = data.end_node->x * w;
        float ey = data.end_node->y * h;
        QColor color = data.trapped ? Qt::yellow : Qt::red;
        m_scene->addEllipse(ex-5, ey-5, 10, 10, QPen(Qt::black), QBrush(color))->setZValue(4);
    }
}

void MainWindow::handleSingleStep() {
    if (!m_sim) return;
    drawTree();
    m_current_path = m_sim->run_single_iteration();
    highlightPath(m_current_path);
    updateStatsDisplay();
}

void MainWindow::handleStartSimulation() {
    if (!m_sim) return;
    for(int i = 0; i < 10000; ++i) m_sim->run_single_iteration();
    updateStatsDisplay();
}

void MainWindow::handleReset() {
    createNewTree();
    m_statsText->clear();
}

void MainWindow::updateStatsDisplay() {
    const auto& stats = m_sim->get_stats();
    QString text = QString("Всего прогонов: %1\n").arg(stats.total_runs);
    text += QString("Застряло: %1\n").arg(stats.trapped_count);
    text += "Длины путей (L):\n";
    for(auto const& [len, count] : stats.length_dist) {
        double prob = (double)count / stats.total_runs;
        text += QString(" L=%1: %2%\n").arg(len).arg(prob * 100, 0, 'f', 1);
    }
    m_statsText->setText(text);
}

MainWindow::~MainWindow() = default;