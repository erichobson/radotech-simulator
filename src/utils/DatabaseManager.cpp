/**
 * @file src/utils/DatabaseManager.cpp
 * @brief
 */

#include "DatabaseManager.h"
#include <QDebug>

DatabaseManager::DatabaseManager() {
    dbConnection = QSqlDatabase::addDatabase("QSQLITE");
    dbConnection.setDatabaseName(QCoreApplication::applicationDirPath() + "/Radotech.db");

    if(dbConnection.open()) {
        qInfo() << "Database Connected";
    }else {
        qInfo() << "Database Not Connected";
        qInfo() << "Error" << dbConnection.lastError();
    }
}


DatabaseManager::~DatabaseManager() {
    qInfo() << "Destructing db manager";
    if(dbConnection.open()) dbConnection.close();
}

