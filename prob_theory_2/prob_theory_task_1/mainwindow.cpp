#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTextEdit>
#include <QGraphicsView>
#include<QWheelEvent>
#include <cmath>
#include<set>

#include <QtOpenGLWidgets/QOpenGLWidget>

QString MainWindow::getStyleSheet() {
    return R"(
        QMainWindow { background-color: #fcf0f2; }

        #sidebar {
            background-color: #ffffff;
            border-right: 0px;
            border-top-right-radius: 30px;
            border-bottom-right-radius: 30px;
        }

        #titleLabel {
            color: #4a4e69;
            font-family: 'Georgia', serif;
            font-size: 24px;
            font-weight: bold;
        }

        QFrame#parameterCard {
            background-color: #fde2e4;
            border-radius: 15px;
            padding: 10px;
        }

        QLabel { color: #9a8c98; font-family: 'Segoe UI'; font-size: 13px; font-weight: 500; }

        QPushButton {
            background-color: #ffb7b2;
            color: #4a4e69;
            border: none;
            border-radius: 12px;
            padding: 12px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #ff9aa2;
        }

        QSlider::groove:horizontal {
            height: 4px;
            background: #e9ecef;
            border-radius: 2px;
        }

        QSlider::handle:horizontal {
            background: #4a4e69;
            width: 14px;
            height: 14px;
            margin: -5px 0;
            border-radius: 7px;
        }

        #graphView {
            background-color: #ffffff;
            border-radius: 25px;
            border: 2px solid #fde2e4;
            margin: 15px;
        }

        #resultsLog {
            background-color: #4a4e69;
            color: #fde2e4;
            border-radius: 20px;
            padding: 15px;
            font-family: 'Consolas';
        }

        QLabel#statBadge {
            background-color: #ffffff;
            border: 1px solid #fde2e4;
            border-radius: 10px;
            padding: 5px 15px;
            color: #4a4e69;
        }
    )";
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setStyleSheet(getStyleSheet());

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    QFrame *sidebar = new QFrame();
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(300);
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);

    QLabel *title = new QLabel("EPIDEMIC\nSIMULATOR");
    title->setObjectName("titleLabel");
    sideLayout->addWidget(title);

    QPushButton *btnLoad = new QPushButton("📂 Import Data");
    sideLayout->addWidget(btnLoad);

    QSlider *p1Slider = new QSlider(Qt::Horizontal);
    p1Slider->setRange(0, 100); p1Slider->setValue(15);
    sideLayout->addWidget(new QLabel("Infection Probability (p1)"));
    sideLayout->addWidget(p1Slider);

    QSlider *p2Slider = new QSlider(Qt::Horizontal);
    p2Slider->setRange(0, 100); p2Slider->setValue(5);
    sideLayout->addWidget(new QLabel("Recovery Probability (p2)"));
    sideLayout->addWidget(p2Slider);

    QPushButton *btnStep = new QPushButton("Next Step ➡");
    sideLayout->addWidget(btnStep);

    QPushButton *btnReset = new QPushButton("Reset Simulation");
    sideLayout->addWidget(btnReset);

    sideLayout->addStretch();

    QPushButton *fBtn1 = new QPushButton("Show Susceptible");
    QPushButton *fBtn2 = new QPushButton("Show Healed + Vulnerable");
    sideLayout->addWidget(fBtn1);
    sideLayout->addWidget(fBtn2);

    mainLayout->addWidget(sidebar);

    QVBoxLayout *rightLayout = new QVBoxLayout();

    QHBoxLayout *statsLayout = new QHBoxLayout();
    lblHealthy = new QLabel("Healthy: 0");
    lblInfected = new QLabel("Infected: 0");
    lblRecovered = new QLabel("Recovered: 0");
    lblHealthy->setObjectName("statBadge");
    lblInfected->setObjectName("statBadge");
    lblRecovered->setObjectName("statBadge");

    statsLayout->addWidget(lblHealthy);
    statsLayout->addWidget(lblInfected);
    statsLayout->addWidget(lblRecovered);
    rightLayout->addLayout(statsLayout);

    graphView = new QGraphicsView();
    scene = new QGraphicsScene(this);
    graphView->setScene(scene);
    graphView->setRenderHint(QPainter::Antialiasing);
    graphView->setDragMode(QGraphicsView::ScrollHandDrag);
    rightLayout->addWidget(graphView, 5);

    resultsLog = new QTextEdit();
    resultsLog->setReadOnly(true);
    rightLayout->addWidget(resultsLog, 2);

    mainLayout->addLayout(rightLayout);

    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadFile);
    connect(btnStep, &QPushButton::clicked, this, &MainWindow::onNextStep);
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::onReset);

    auto updateProbs = [this, p1Slider, p2Slider]() {
        sim.set_probabilities(p1Slider->value()/100.0, p2Slider->value()/100.0);
    };
    connect(p1Slider, &QSlider::valueChanged, updateProbs);
    connect(p2Slider, &QSlider::valueChanged, updateProbs);

    connect(fBtn1, &QPushButton::clicked, [this](){ applyFilter(0); });
    connect(fBtn2, &QPushButton::clicked, [this](){ applyFilter(2); });


    graphView->setViewport(new QOpenGLWidget());
    graphView->setRenderHint(QPainter::Antialiasing, true);
    graphView->setDragMode(QGraphicsView::ScrollHandDrag);
    graphView->setOptimizationFlag(QGraphicsView::DontSavePainterState);
    graphView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

}

void MainWindow::onLoadFile() {
    QString path = QFileDialog::getOpenFileName(this, "Select Network File", "", "Text (*.txt);;All files (*.*)");
    if (path.isEmpty()) return;

    try {
        sim.load_edge_list(path.toStdString());
        sim.seed_random(1);
        setupGraphLayout();
        resultsLog->append("✅ Loaded " + QString::number(sim.size()) + " nodes.");
        updateStats();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::setupGraphLayout() {
    if (sim.empty()) return;
    scene->clear();
    nodeItems.clear();

    const auto& nodes = sim.nodes();
    int N = nodes.size();
    QVector<QPointF> pos(N);

    for(int i = 0; i < N; ++i) {
        pos[i] = QPointF((rand() % 800) - 400, (rand() % 800) - 400);
    }

    double area = 1000000.0;
    double k = std::sqrt(area / N) * 1.5;
    double temp = 100.0;

    for (int iter = 0; iter < 50; ++iter) {
        QVector<QPointF> disp(N, QPointF(0, 0));

        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; j += 5) {
                QPointF diff = pos[i] - pos[j];
                double dist = std::sqrt(diff.x()*diff.x() + diff.y()*diff.y()) + 0.1;
                if (dist < k * 5) {
                    QPointF f = (diff / dist) * (k * k / dist);
                    disp[i] += f;
                    disp[j] -= f;
                }
            }
        }

        for (const auto& node : nodes) {
            for (uint32_t targetId : node.neighbours) {
                if (targetId >= (uint32_t)N) continue;
                QPointF diff = pos[node.id] - pos[targetId];
                double dist = std::sqrt(diff.x()*diff.x() + diff.y()*diff.y()) + 0.1;
                QPointF f = (diff / dist) * (dist * dist / k);
                disp[node.id] -= f;
                disp[targetId] += f;
            }
        }

        for (int i = 0; i < N; ++i) {
            double dLen = std::sqrt(disp[i].x()*disp[i].x() + disp[i].y()*disp[i].y()) + 0.1;
            pos[i] += (disp[i] / dLen) * std::min(dLen, temp);
        }
        temp *= 0.95;
    }

    int V = nodes.size();

    QRectF bounds = scene->itemsBoundingRect();
    if (bounds.isEmpty()) bounds = QRectF(-500, -500, 1000, 1000);

    double viewScale = std::min(bounds.width(), bounds.height()) / 1000.0;
    if (viewScale < 0.1) viewScale = 1.0;

    double nodeBaseSize;
    double edgeOpacity;
    double edgeWidth;

    if (V < 100) {
        nodeBaseSize = 20.0;
        edgeOpacity = 180;
        edgeWidth = 2.0;
    } else {
        nodeBaseSize = 12.0 / std::sqrt(V / 100.0);
        edgeOpacity = (V > 1000) ? 25 : 60;
        edgeWidth = (V > 1000) ? 0.5 : 1.0;
    }

    nodeBaseSize *= viewScale;

    QPen edgePen(QColor(74, 78, 105, edgeOpacity));
    edgePen.setWidthF(edgeWidth * viewScale);

    for (const auto& node : nodes) {
        for (uint32_t targetId : node.neighbours) {
            if (targetId > node.id) {
                scene->addLine(pos[node.id].x(), pos[node.id].y(),
                               pos[targetId].x(), pos[targetId].y(), edgePen);
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        double currentSize = (nodes[i].neighbours.size() > 15) ? nodeBaseSize * 2.2 : nodeBaseSize;

        QPen nodePen(QColor(40, 40, 60));
        nodePen.setWidthF((V > 1000) ? 0.2 : 1.5);

        auto* circle = scene->addEllipse(-currentSize/2, -currentSize/2,
                                         currentSize, currentSize,
                                         nodePen, Qt::white);
        circle->setPos(pos[i]);
        circle->setZValue(10);
        nodeItems[nodes[i].id] = circle;
    }

    scene->setSceneRect(scene->itemsBoundingRect());
    graphView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    updateGraphColors();
}
void MainWindow::onNextStep() {
    if (sim.empty()) return;
    sim.step();
    updateGraphColors();
    updateStats();
}

void MainWindow::onReset() {
    for(auto& node : sim.nodes()) {
        node.state = health_t::healthy;
    }
    sim.seed_random(1);
    updateGraphColors();
    updateStats();
    resultsLog->append("🔄 Simulation reset.");
}

void MainWindow::updateStats() {
    lblHealthy->setText(QString("Healthy: %1").arg(sim.count_susceptible()));
    lblInfected->setText(QString("Infected: %1").arg(sim.count_infected()));
    lblRecovered->setText(QString("Recovered: %1").arg(sim.count_recovered()));
}

void MainWindow::updateGraphColors() {
    for(const auto& p : sim.nodes()) {
        if(!nodeItems.contains(p.id)) continue;

        QColor color;
        switch(p.state) {
        case health_t::healthy:
            color = QColor("#ffffff");
            break;
        case health_t::infected:
            color = QColor("#ff9aa2");
            break;
        case health_t::recovered:
            color = QColor("#b5ead7");
            break;
        }
        nodeItems[p.id]->setBrush(QBrush(color));
    }
}

void MainWindow::applyFilter(int type) {
    if (sim.empty()) return;
    std::vector<uint32_t> ids;
    QString name;

    if (type == 0) { ids = sim.get_all_susceptible(); name = "Susceptible"; }
    else { ids = sim.get_recovered_with_vulnerable_neighbours(); name = "Recovered w/ Neighbors"; }

    resultsLog->append(QString("🔍 Filter [%1]: Found %2 nodes").arg(name).arg(ids.size()));

    for(auto* item : nodeItems.values()) item->setOpacity(0.1);
    for(uint32_t id : ids) {
        if(nodeItems.contains(id)) {
            nodeItems[id]->setOpacity(1.0);
            nodeItems[id]->setZValue(10);
        }
    }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        const double scaleFactor = 1.15;
        if (event->angleDelta().y() > 0)
            graphView->scale(scaleFactor, scaleFactor);
        else
            graphView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        event->accept();
    } else {
        QMainWindow::wheelEvent(event);
    }
}
MainWindow::~MainWindow() {}
