//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_1_STATISTICS_PANEL_HPP
#define PROB_THEORY_3_1_STATISTICS_PANEL_HPP

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QGroupBox>

class statistics_panel : public QWidget {
    Q_OBJECT

public:
    explicit statistics_panel(QWidget *parent = nullptr);

public slots:
    void update_stats(double probA, double probB, double probC, double avgConn, size_t currentIter);
    void set_progress(int percent);
    void clear();

private:
    QProgressBar* m_progressBar;
    QLabel* m_probALabel;
    QLabel* m_probBLabel;
    QLabel* m_probCLabel;
    QLabel* m_avgConnLabel;
    QLabel* m_iterLabel;

    void setup_ui();
    QLabel* create_value_label();
};


#endif //PROB_THEORY_3_1_STATISTICS_PANEL_HPP
