/**
 * @file HistoryWidget.h
 * @brief Declaration of the HistoryWidget class.
 */

#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QList>
#include <QWidget>

#include "ResultsWidget.h"
#include "ScanModel.h"

class QGridLayout;
class QVBoxLayout;
class QPushButton;
class QScrollArea;
class UserProfileController;

class HistoryWidget : public QWidget {
    Q_OBJECT

   public:
    explicit HistoryWidget(
        QWidget* parent = nullptr,
        UserProfileController* userProfileController = nullptr);

   protected:
    /**
     * @brief Event filter to handle custom events.
     * @param obj The object receiving the event.
     * @param event The event being received.
     * @return True if the event was handled; otherwise false.
     */
    bool eventFilter(QObject* obj, QEvent* event) override;

   public slots:
    void setCurrentProfile(int profileId);
    void onNewScanStored(const ScanModel& scan);

   private slots:
    void onScanSelected(ScanModel* scan);
    void onBackToHistoryClicked();

   private:
    void setupUI();
    void loadScansForProfile();
    void displayScans();
    void showResultsView(ScanModel* scan);
    void showHistoryView();

    UserProfileController* userProfileController;
    int currentProfileId;
    QVector<ScanModel*> profileScans;

    QWidget* historyView;
    ResultsWidget* resultsView;
    QVBoxLayout* mainLayout;
    QVBoxLayout* scansGrid;
    QScrollArea* scrollArea;
};

#endif  // HISTORYWIDGET_H
