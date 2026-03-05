//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_1_VISUAL_PANEL_HPP
#define PROB_THEORY_3_1_VISUAL_PANEL_HPP

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include "simulator_worker.hpp"

class visual_panel : public QWidget{
    Q_OBJECT

public:
    explicit visual_panel(QWidget *parent = nullptr);

public slots:
    void display_iteration(const iteration_data& data);
    void clear();

private:
    QLabel* m_headerLabel;
    QTextEdit* m_display;
    void setup_ui();
};


#endif //PROB_THEORY_3_1_VISUAL_PANEL_HPP
