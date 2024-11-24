/**
 * @file MainWindow.h
 * @brief Declaration of the MainWindow class.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class LoginWidget;
class QStackedWidget;

/**
 * @class MainWindow
 * @brief The MainWindow class represents the main application window.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    /**
     * @brief Constructs the MainWindow.
     * @param parent The parent widget. Defaults to nullptr.
     */
    explicit MainWindow(QWidget *parent = nullptr);

   private slots:
    /**
     * @brief Slot called when a login is requested.
     * @param username The entered username.
     * @param password The entered password.
     */
    void onLoginRequested(const QString &username, const QString &password);

    /**
     * @brief Slot called when a registration is requested.
     */
    void onRegisterRequested();

    /**
     * @brief Logs out the current user.
     */
    void logout();

   private:
    QStackedWidget *stackedWidget;
    LoginWidget *loginWidget;
    QWidget *mainPageWidget;
    QTabWidget *tabWidget;
};

#endif  // MAINWINDOW_H
