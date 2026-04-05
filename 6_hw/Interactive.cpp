//
// Created by maria on 05.04.2026.
//

#include "Interactive.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsPathItem>

Interactive::Interactive(QWidget* parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);

    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    setStyleSheet("border: none; background: transparent;");
    setBackgroundBrush(QColor(255, 255, 255));

    centerOn(0, 0);
}
void Interactive::setGenerators(const QVector<Generator>& gens)
{
    m_generators = gens;
    redrawScene();
}
void Interactive::wheelEvent(QWheelEvent* event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFacor = 1.5;
    if(event->angleDelta().y() > 0)
    {
        scale(scaleFacor, scaleFacor);
    }
    else
    {
        scale(1.0 / scaleFacor, 1.0/scaleFacor);
    }
}
void Interactive::redrawScene()
{
    scene->clear();

    const int unit = 40;
    const int maxGrid = 50;
    const int minGrid = -5;

    QPen gridPen(QColor(240, 240, 245), 1);
    QPen axisPen(QColor(180, 185, 195), 2);

    for (int i = minGrid; i <= maxGrid; ++i)
    {
        scene->addLine(i * unit, -maxGrid * unit, i * unit, -minGrid * unit, gridPen);
        scene->addLine(minGrid * unit, -i * unit, maxGrid * unit, -i * unit, gridPen);
    }

    scene->addLine(minGrid * unit, 0, maxGrid * unit, 0, axisPen);
    scene->addLine(0, -maxGrid * unit, 0, -minGrid * unit, axisPen);

    QFont textFont("Segoe UI", 9);
    QColor textColor(140, 145, 155);

    for (int i = minGrid; i <= maxGrid; ++i)
    {
        if (i == 0) continue;
        auto *textX = scene->addText(QString::number(i), textFont);
        textX->setDefaultTextColor(textColor);
        textX->setPos(i * unit - (textX->boundingRect().width() / 2), 5);
        textX->setFlag(QGraphicsItem::ItemIgnoresTransformations);

        auto *textY = scene->addText(QString::number(i), textFont);
        textY->setDefaultTextColor(textColor);
        textY->setPos(-25, -i * unit - 10);
        textY->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    }

    auto *textZero = scene->addText("0", textFont);
    textZero->setDefaultTextColor(textColor);
    textZero->setPos(-15, 5);
    textZero->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    if (m_generators.isEmpty()) return;

    QPainterPath idealPath;
    for (const auto& g : m_generators)
    {
        QPainterPath rectPath;
        int rectX = g.a * unit;
        int rectY = -maxGrid * unit;
        int rectW = (maxGrid - g.a) * unit;
        int rectH = (maxGrid - g.b) * unit;
        rectPath.addRect(rectX, rectY, rectW, rectH);
        idealPath = idealPath.united(rectPath);
    }

    QColor idealColor(163, 196, 188, 140);
    scene->addPath(idealPath, Qt::NoPen, idealColor);

    QColor pointColor(226, 149, 120);
    QPen pointPen(Qt::white, 2);

    for (const auto& g : m_generators)
    {
        int px = g.a * unit;
        int py = -g.b * unit;
        scene->addEllipse(px - 6, py - 6, 12, 12, pointPen, pointColor);
    }
}
