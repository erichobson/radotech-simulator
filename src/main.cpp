/**
 * @file src/main.cpp
 * @brief Entry point for the RaDoTech simulation application.
 */

#include <QApplication>

#include "MainWindow.h"

#ifdef QT_DEBUG
#include "DatabaseManager.h"
#include "DatabaseManagerTest.h"
#include "HealthMetricCalculatorTest.h"
#include "Logging.h"
#include "ProfileModelTest.h"
#include "ScanModelTest.h"
#include "Test.h"
#include "UserModelTest.h"
#include "UserProfileControllerTest.h"
#include "UserControllerTest.h"
#endif

/**
 * @brief Main function of the application.
 *
 * Initializes the Qt application and displays the main window.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Exit status of the application.
 */
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

#ifdef QT_DEBUG
    DEBUG("\n***Start testing***\n");
    DatabaseManager db;
    QVector<Test*> tests = {
        new DatabaseManagerTest(db),      new UserModelTest(),
        new ProfileModelTest(),           new ScanModelTest(),
        new HealthMetricCalculatorTest(), new UserProfileControllerTest(db),
        new UserControllerTest(db)
    };

    // Run & delete tests
    for (const auto* test : tests) {
        if (!test->test()) {
            ERROR("One of the tests failed");
        }
    }

    for (const auto* test : tests) {
        delete test;
    }

    DEBUG("\n***Done testing***\n");
#endif

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
