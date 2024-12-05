/**
 * @file ResultsWidget.h
 * @brief
 */

#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QVBoxLayout>
#include <QWidget>

#include "HealthMetricCalculator.h"
#include "ScanModel.h"

class ResultsWidget : public QWidget {
    Q_OBJECT

   public:
    explicit ResultsWidget(QWidget* parent = nullptr);
    void setScanModel(const ScanModel& scanModel);
    void setShowBackButton(bool show) { showBackButton = show; }

   signals:
    void backButtonClicked();

   private:
    void setupUI();
    void displayResults();
    void displayGeneralHealthParameters();

    QVBoxLayout* mainLayout;
    QVBoxLayout* cardLayout;
    bool showBackButton = false;

    ScanModel currentScan;
    HealthMetricCalculator calculator;
    QStringList measurementLabels;

    QVector<HealthMetricModel*> healthMetrics;
};

#endif  // RESULTSWIDGET_H
