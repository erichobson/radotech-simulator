/**
 * @file HistoryWidget.h
 * @brief Declaration of the HistoryWidget class.
 */

#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>

class HistoryWidget : public QWidget {
    Q_OBJECT
   public:
    explicit HistoryWidget(QWidget *parent = nullptr);

   private slots:
    /**
     * @brief Slot to handle the "View Previous Scans" button click
     */
    void onViewPreviousScansButtonClicked();

   private:
    void createTrendWidgets();
    void createViewHistoryButton();
};

#endif  // HISTORYWIDGET_H
