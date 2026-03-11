//
// Created by mvideo on 07/03/26.
//

#ifndef PROB_THEORY_HUB_DRANKMANWINDOW_HPP
#define PROB_THEORY_HUB_DRANKMANWINDOW_HPP

#include "../base_ui/UniversalBaseWindow.hpp"
#include "DrunkardEngine.hpp"
class DrankManWindow : public UniversalBaseWindow
{
    Q_OBJECT
public:
    explicit DrankManWindow(QWidget* parent = nullptr);
    virtual ~DrankManWindow();

protected:
    void onSingleStep() override;
    void onBatchStep() override;
    void onResetStep() override;
    void onPageChanged(int index) override;
    void runCalculation(const QJsonObject& config) override;
private:
    void updateUI();
    void drawTrial(int index);
    void syncEngine();
    static QMap<QString, ControlSchema> createSchema();

    DrunkardEngine* m_engine;
    QVector<TrialResult> m_allTrials;

    void onPrevPage() override;
    void onNextPage() override;
};


#endif //PROB_THEORY_HUB_DRANKMANWINDOW_HPP
