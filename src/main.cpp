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
    
    int h1 = 55;
    int h2 = 56;
    int h3 = 57;
    int h4 = 58;
    int h5 = 59;
    int h6 = 55;
    
    int h1r = 55;
    int h2r = 56;
    int h3r = 57;
    int h4r = 58;
    int h5r = 59;
    int h6r = 55;
    
    int f1 = 55;
    int f2 = 22;
    int f3 = 75;
    int f4 = 58;
    int f5 = 25;
    int f6 = 27;
    
    int f1r = 75;
    int f2r = 52;
    int f3r = 57;
    int f4r = 58;
    int f5r = 25;
    int f6r = 20;

    int bt = 150;
    int bp = 160;
    int hr = 188;
    int st = 2232;
    int cw = 185;
    int em = 1;
    int ofeel = 2;
    QString name = "Andrew scan";
    QString notes = "Test note";

    ScanModel newScan(
        id,profileId,
        h1,h1r,h2,h2r,h3,h3r,h4,h4r,h5,h5r,h6,h6r,
        f1,f1r,f2,f2r,f3,f3r,f4,f4r,f5,f5r,f6,f6r,
        bt,bp,hr,st,cw,em,ofeel,name,notes
    );

    qDebug() << "Calculating Organ Health";
    hmc.calculateOrganHealth(&newScan, hms);

    qDebug() << "Calculating Indicator Health";
    hmc.calculateIndicatorHealth(&newScan, hms);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
