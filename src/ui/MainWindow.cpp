/**
 * @file src/ui/MainWindow.cpp
 * @brief
 */

#include "MainWindow.h"
#include "DatabaseManager.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("RaDoTech");
    resize(800, 600);

    DatabaseManager db;
    db.testCRUD();

}
