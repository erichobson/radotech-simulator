/**
 * @file src/main.cpp
 * @brief Entry point for the RaDoTech simulation application.
 */

#include <QApplication>

#include "ui/MainWindow.h"
#include "DatabaseManager.h"
#include "UserModel.h"
#include "ProfileModel.h"
#include "ScanModel.h"
#include "HealthMetricCalculator.h"
#include "HealthMetricModel.h"

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
    UserModel user;
    ProfileModel profile;
    ScanModel scan;
    HealthMetricCalculator hmc;

    db.testCRUD();
    user.test();
    profile.test();
    scan.test();

    QVector<HealthMetricModel*> hms;
    int id = -1;
    int profileId = 2;
    int h1 = 255;
    int h2 = 256;
    int h3 = 257;
    int h4 = 258;
    int h5 = 259;
    int h6 = 255;
    int f1 = 255;
    int f2 = 277;
    int f3 = -1000;
    int f4 = 258;
    int f5 = 200;
    int f6 = 2000;
    int bt = 150;
    int bp = 160;
    int hr = 188;
    int st = 2232;
    int cw = 185;
    int em = 1;
    int ofeel = 2;
    QString name = "Andrew scan";
    QString notes = "Test note";

    ScanModel newScan(id,profileId,h1,h2,h3,h4,h5,h6,f1,f2,f3,f4,f5,f6,bt,bp,hr,st,cw,em,ofeel,name,notes);
    hmc.calculateOrganHealth(&newScan, hms);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
