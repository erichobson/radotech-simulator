/**
 * @file src/main.cpp
 * @brief Entry point for the RaDoTech simulation application.
 */

#include <QApplication>

#include "ui/MainWindow.h"

#include "Test.h"
#include "DatabaseManagerTest.h"
#include "DatabaseManager.h"
#include "UserModelTest.h"
#include "ProfileModelTest.h"
#include "ScanModelTest.h"
#include "HealthMetricCalculatorTest.h"
#include "UserProfileControllerTest.h"
#include <QDebug>

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

    DatabaseManager db;
    QVector<Test*> tests = {
        new DatabaseManagerTest(db),
        new UserModelTest(),
        new ProfileModelTest(),
        new ScanModelTest(),
        new HealthMetricCalculatorTest(),
        new UserProfileControllerTest(db)
    };

    // Run & delete tests
    for(const auto* test : tests){
        if(!test->test()) {
            qDebug() << "One of the tests failed";
        }
    }
    
    for(const auto* test : tests){
        delete test;
    }
    
    qDebug() << "\n***Done testing***\n";

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
