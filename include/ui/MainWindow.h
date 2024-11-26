/**
 * @file MainWindow.h
 * @brief Declaration of the MainWindow class.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

class QStackedWidget;
class LoginWidget;
class QListWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);

   private slots:
    /**
     * @brief Handles the resize event to update the background gradient.
     * @param event The resize event.
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief Slot called when a login is requested.
     * @param username The username entered.
     * @param password The password entered.
     */
    void onLoginRequested(const QString &username, const QString &password);

    /**
     * @brief Slot called when a registration is requested.
     */
    void onRegisterRequested();

    /**
     * @brief Logs out the current user and returns to the login page.
     */
    void logout();

   private:
    QStackedWidget *stackedWidget;
    LoginWidget *loginWidget;
    QWidget *mainWidget;

    QListWidget *sidebarMenu;
    QStackedWidget *contentStackedWidget;
    QVector<QWidget *> contentWidgets;

    struct ItemInfo {
        QString iconPathUnselected;
        QString iconPathSelected;
        QString title;
    };

    QVector<ItemInfo> items;
};

#endif  // MAINWINDOW_H
