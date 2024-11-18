/**
 * @file src/main.cpp
 * @brief Entry point for the RaDoTech simulation application.
 */

#include <QApplication>

#include "ui/MainWindow.h"

/**
 * @brief Main function of the application.
 *
 * Initializes the Qt application and displays the main window.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Exit status of the application.
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
