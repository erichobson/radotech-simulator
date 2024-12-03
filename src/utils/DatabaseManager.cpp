/**
 * @file DatabaseManager.cpp
 * @brief Responsible for managing the database connection and executing queries.
 */

#include "DatabaseManager.h"
#include <QDebug>

DatabaseManager::DatabaseManager() {
    Q_INIT_RESOURCE(resources);
    init();
}

DatabaseManager::~DatabaseManager() {
    qInfo() << "Destructing db manager";
    if(dbConnection.open()) dbConnection.close();
}

void DatabaseManager::execute(const QString& query, const QList<QVariant>& params) {
    QSqlQuery sqlQuery(dbConnection);

    if(!sqlQuery.prepare(query)) handleError(sqlQuery.lastError());

    for(int i = 0; i < params.size(); ++i) sqlQuery.bindValue(i, params[i]);

    if(!sqlQuery.exec()) handleError(sqlQuery.lastError());
}

void DatabaseManager::query(const QString& query, const QList<QVariant>& params, QList<QMap<QString, QVariant>>& results) {
    QSqlQuery sqlQuery(dbConnection);

    if(!sqlQuery.prepare(query)) handleError(sqlQuery.lastError());

    for(int i = 0; i < params.size(); ++i) sqlQuery.bindValue(i, params[i]);

    if(!sqlQuery.exec()) handleError(sqlQuery.lastError());

    results.clear();
    while(sqlQuery.next()) {
        QMap<QString, QVariant> row;
        for (int i = 0; i < sqlQuery.record().count(); ++i) {
            row.insert(sqlQuery.record().fieldName(i), sqlQuery.value(i));
        }
        results.append(row);
    }
}

void DatabaseManager::handleError(const QSqlError& error) {
    throw std::runtime_error("Database error: " + error.text().toStdString());
}

void DatabaseManager::executeSqlScript(const QString& filePath, QSqlDatabase& db) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open SQL file:" << file.errorString();
        return;
    }

    QTextStream stream(&file);
    QString sqlQuery = stream.readAll();
    file.close();

    QStringList commands = sqlQuery.split(";", Qt::SkipEmptyParts);
    QSqlQuery query(db);

    for (const QString& command : commands) {
        QString trimmedCommand = command.trimmed();
        if (!trimmedCommand.isEmpty()) {
            if (!query.exec(trimmedCommand)) {
                qDebug() << "SQL Error:" << query.lastError().text() << "\nCommand:" << trimmedCommand;
            }
        }
    }
}

void DatabaseManager::init() {
    dbConnection = QSqlDatabase::addDatabase("QSQLITE");
    dbConnection.setDatabaseName(QCoreApplication::applicationDirPath() + "/Radotech.db");

    if (!dbConnection.open()) {
        qDebug() << "Failed to open database:" << dbConnection.lastError().text();
        return;
    } else {
        qDebug() << "Database connection successful";
    }

    executeSqlScript(":/sql/schema.sql", dbConnection);
    executeSqlScript(":/sql/dummy_data.sql", dbConnection);
}

bool DatabaseManager::isConnectionOpen() {
    return dbConnection.isOpen();
}

