//
// Created by mvideo on 25/03/26.
//

#ifndef PROB_THEORY_6_WIDGET_HPP
#define PROB_THEORY_6_WIDGET_HPP

#include<QWidget>
#include<QPainter>
#include <vector>
#include "../Core/IExperiment.hpp"
#include <random>

struct VisualNeedle
{
    QPointF p1;
    QPointF p2;
    bool crosses;
};

class CWidget : public QWidget
{
    Q_OBJECT

private:
    std::vector<VisualNeedle> m_visualNeedles;
    double m_d;
    double m_L;

public:
    explicit CWidget(QWidget* parent = nullptr) : QWidget(parent), m_d(0), m_L(0){
        setMinimumSize(2000, 2000);
    }
    void updateData(const std::vector<NeedleResult>& data,
                    double d,
                    double L)
    {
        m_L = L;
        m_d = d;
        generateVisuals(data);
        update();
    }

    void generateVisuals(const std::vector<NeedleResult>& data)
    {
        m_visualNeedles.clear();
        if(data.empty() || m_d <= 0) return;

        int numLines = height() / m_d + 2;
        if (numLines < 2) numLines = 2;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> distX(0.0, width());
        std::uniform_int_distribution<int> distLine(1, numLines - 2);
        std::uniform_int_distribution<int> distSign(0, 1);

        for(const auto& res : data)
        {
            double cx = distX(gen);
            int lineIdx = distLine(gen);

            double sign = distSign(gen) == 0 ? 1.0 : -1.0;
            double cy = lineIdx * m_d + sign * res.x;

            double angle = res.phi;
            if (distSign(gen) == 0) angle = -angle;

            double dx = (m_L / 2.0) * std::cos(angle);
            double dy = (m_L / 2.0) * std::sin(angle);

            if (distSign(gen) == 0) {
                dx = -dx;
                dy = -dy;
            }

            VisualNeedle vn;

            vn.p1 = QPointF(cx - dx, cy - dy);
            vn.p2 = QPointF(cx + dx, cy + dy);
            vn.crosses = res.crosses;

            m_visualNeedles.push_back(vn);
        }
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), Qt::white);

        if (m_d <= 0 || m_visualNeedles.empty()) return;

        int numLines = height() / m_d + 2;
        painter.setPen(QPen(Qt::blue, 1));
        for (int i = 0; i < numLines; ++i) {
            double y = i * m_d;
            painter.drawLine(0, y, width(), y);
        }

        const size_t maxDrawCount = 2000;
        size_t drawCount = std::min(m_visualNeedles. size(), maxDrawCount);

        for(size_t i = 0; i < drawCount; ++i)
        {
            const auto& vn = m_visualNeedles[i];
            painter.setPen(QPen(vn.crosses ? Qt::red : Qt::green, 2));
            painter.drawLine(vn.p1, vn.p2);
        }

        if (m_visualNeedles.size() > maxDrawCount)
        {
            painter.setPen(Qt::black);
            painter.drawText(10, 20, QString("Showing %1 out of %2 needles (performance limit)")
                    .arg(maxDrawCount).arg(m_visualNeedles.size()));
        }
    }
};

#endif //PROB_THEORY_6_WIDGET_HPP
