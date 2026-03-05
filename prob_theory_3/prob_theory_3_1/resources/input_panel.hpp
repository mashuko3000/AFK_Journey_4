//
// Created by mvideo on 01/03/26.
//

#ifndef PROB_THEORY_3_1_INPUT_PANEL_HPP
#define PROB_THEORY_3_1_INPUT_PANEL_HPP

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QGridLayout>

#include "simulator_worker.hpp"

class input_panel : public QWidget{
    Q_OBJECT

public:
    explicit input_panel(QWidget *parent = nullptr);
    simulation_config get_config() const;
    void set_controls_enabled(bool enabled);

signals:
    void start_requested(const simulation_config& config);
    void step_requested(const simulation_config& config);
    void stop_requested();
    void reset_requested();
    void batch_step_requested(const simulation_config& config, int batchSize);

private slots:
    void on_alphabet_changed(const QString& new_alphabet);
    void on_dist_type_changed(int index);
    void generate_random_weights();

private:
    QLabel* m_titleLabel;

    QGroupBox* m_alphaGroup;
    QLineEdit* m_alphabetEdit;
    QComboBox* m_distTypeCombo;
    QTableWidget* m_weightsTable;
    QPushButton* m_randomizeWeightsBtn;

    QGroupBox* m_paramsGroup;

    QSpinBox* m_mSpin;
    QSlider* m_mSlider;

    QSpinBox* m_nSpin;
    QSlider* m_nSlider;

    QSpinBox* m_dSpin;
    QSlider* m_dSlider;

    QLineEdit* m_patternEdit;

    QGroupBox* m_controlGroup;
    QSpinBox* m_iterSpin;
    QPushButton* m_startBtn;
    QPushButton* m_stepBtn;
    QPushButton* m_batchStepBtn;
    QSpinBox* m_batchSizeSpin;
    QPushButton* m_stopBtn;
    QPushButton* m_resetBtn;

    void setup_ui();
    void setup_connections();

    void add_labeled_slider(QGridLayout* layout, const QString& label,
                            QSpinBox* spin, QSlider* slider, int row);
};

#endif //PROB_THEORY_3_1_INPUT_PANEL_HPP
