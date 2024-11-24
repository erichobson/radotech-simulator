/**
 * @file src/ui/MainWindow.cpp
 * @brief
 */

#include "MainWindow.h"
#include "DatabaseManager.h"
#include "UserModel.h"
#include "ProfileModel.h"
#include "ScanModel.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("RaDoTech");
    resize(800, 600);

    DatabaseManager db;
    UserModel user;
    ProfileModel profile;
    ScanModel scan;

    user.test();
    profile.test();
    scan.test();
}
