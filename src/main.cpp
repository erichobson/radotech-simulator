/**
 * @file src/main.cpp
 * @brief Entry point for the RaDoTech simulation application.
 */

#include <QApplication>

#include "ui/MainWindow.h"

#include "Test.h"
#include "DatabaseManagerTest.h"
#include "UserModelTest.h"
#include "ProfileModelTest.h"
#include "ScanModelTest.h"
#include "HealthMetricCalculatorTest.h"

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
        
    QVector<Test*> tests = {
        new DatabaseManagerTest(),
        new UserModelTest(),
        new ProfileModelTest(),
        new ScanModelTest(),
        new HealthMetricCalculatorTest()
    };

    // Run & delete tests
    for(const auto* test : tests){
        test->test();
        delete test;
    }

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
