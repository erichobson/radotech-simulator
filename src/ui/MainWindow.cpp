/**
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class.
 */

#include "MainWindow.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include "DatabaseManager.h"
#include "LoginWidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Create the stacked widget
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Initialize LoginWidget
    loginWidget = new LoginWidget;

    // Initialize DatabaseManager
    DatabaseManager db;

    // Set title and initial size
    setWindowTitle("RaDoTech");
    resize(800, 600);

    // Connect signals from LoginWidget to MainWindow slots
    connect(loginWidget, &LoginWidget::loginRequested, this,
            &MainWindow::onLoginRequested);
    connect(loginWidget, &LoginWidget::registerRequested, this,
            &MainWindow::onRegisterRequested);

    // Create main page (UI setup)
    mainPageWidget = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout;

    // TODO: Incorporate other tabs
    tabWidget = new QTabWidget;
    tabWidget->addTab(new QTabWidget(), "Temp");

    // Add tab widget to main layout
    mainLayout->addWidget(tabWidget);
    mainPageWidget->setLayout(mainLayout);

    // Add pages to the stacked widget
    stackedWidget->addWidget(loginWidget);
    stackedWidget->addWidget(mainPageWidget);

    // Show the login page first
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::onLoginRequested(const QString &username,
                                  const QString &password) {
    // TODO: Incorporate database check
    // Test Verification
    if (username == "user@domain.tld" && password == "password") {
        stackedWidget->setCurrentIndex(1);  // Switch to main page

        // Clear the login fields
        loginWidget->clearFields();
    } else {
        // Show error message on LoginWidget
        loginWidget->setStatusMessage("User not found");
    }
}

void MainWindow::onRegisterRequested() {
    // TODO: Verify the received information and continue to database
    // integration
}

void MainWindow::logout() {
    // TODO: Implement logout functionality
}
