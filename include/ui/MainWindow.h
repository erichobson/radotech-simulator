/**
 * @file MainWindow.h
 * @brief Declaration of the MainWindow class.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "ClickableLabel.h"
#include "DatabaseManager.h"
#include "DeviceController.h"
#include "HistoryWidget.h"
#include "HomeWidget.h"
#include "MeasureNowWidget.h"
#include "ProfileWidget.h"
#include "ProfilesWidget.h"
#include "ScanController.h"
#include "UserController.h"
#include "UserProfileController.h"

class QStackedWidget;
class QVBoxLayout;
class QLabel;
class LoginWidget;
class QListWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);

   protected:
    void resizeEvent(QResizeEvent *event) override;

   public slots:
    void setCurrentProfile(int profileId, const QString &profileName);
    int getCurrentProfileId() const { return currentProfileId; }
    QString getCurrentProfileName() const { return currentProfileName; }

   signals:
    void currentProfileChanged(int profileId, const QString &profileName);

   public slots:
    /**
     * @brief Slot called when a login is requested.
     * @param username The username entered.
     * @param password The password entered.
     */
    void onLoginRequested(const QString &username, const QString &password);

    /**
     * @brief Slot called when a registration is requested.
     */
    void onRegisterRequested(const QString &firstName, const QString &lastName,
                             const QString &sex, const QString &weight,
                             const QString &height, const QDate &dob,
                             const QString &email, const QString &password,
                             const QString &confirmPassword);

    /**
     * @brief Logs out the current user and returns to the login page.
     */
    void logout();

    /**
     * @brief Updates the connection status label.
     * @param isConnected True if connected, false otherwise.
     */
    void updateConnectionStatus(bool isConnected);

    /**
     * @brief Updates the battery percentage label.
     * @param batteryLevel The current battery level.
     */
    void updateBatteryPercentage(int batteryLevel);

    /**
     * @brief Slot called when the battery percentage label is clicked.
     */
    void onBatteryPercentageClicked();

   private:
    /**
     * @brief Sets up the battery information widget.
     */
    void setupBatteryInfoWidget();

    DatabaseManager *databaseManager;
    DeviceController *deviceController;
    UserProfileController *userProfileController;
    ProfileWidget *profileWidget;
    ScanController *scanController;
    UserController *userController;
    ProfilesWidget *profilesWidget;
    HistoryWidget *historyWidget;
    HomeWidget *homeWidget;
    MeasureNowWidget *measureNowWidget;

    int loggedInUserId;
    int currentProfileId;
    QString currentProfileName;

    QStackedWidget *stackedWidget;
    LoginWidget *loginWidget;
    QWidget *mainWidget;

    QVBoxLayout *sidebarLayout;
    QListWidget *sidebarMenu;

    QStackedWidget *contentStackedWidget;
    QVector<QWidget *> contentWidgets;

    QLabel *connectionStatusLabel;
    ClickableLabel *batteryPercentageLabel;

    struct ItemInfo {
        QString iconPathUnselected;
        QString iconPathSelected;
        QString title;
    };
    QVector<ItemInfo> items;
};

#endif  // MAINWINDOW_H
