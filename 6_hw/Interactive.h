//
// Created by maria on 05.04.2026.
//

#ifndef INC_6_HW_INTERACTIVE_H
#define INC_6_HW_INTERACTIVE_H

#include<QGraphicsView>
#include <QVector>

struct Generator
{
    int a;
    int b;
};

class Interactive : public QGraphicsView
{
    QGraphicsScene *scene;
    QVector<Generator> m_generators;

public:
    explicit Interactive(QWidget* parent = nullptr);
    void setGenerators(const QVector<Generator>& gens);

protected:
    void wheelEvent(QWheelEvent* event) override;

private:
    void redrawScene();
};


#endif //INC_6_HW_INTERACTIVE_H
