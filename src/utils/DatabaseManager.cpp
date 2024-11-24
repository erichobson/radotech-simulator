/**
 * @file src/utils/DatabaseManager.cpp
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

void DatabaseManager::testCRUD() {
    const QString userQuery = "SELECT * FROM users;";
    const QString profileQuery = "SELECT * FROM profile;";
    const QString scanQuery = "SELECT * FROM scan;";

    QList<QMap<QString, QVariant>> userRes;
    QList<QMap<QString, QVariant>> profileRes;
    QList<QMap<QString, QVariant>> scanRes;

    // Create
    qDebug() << "\nCREATING USER " << "Test User 2" <<" ******************";
    execute("INSERT INTO users (first_name, last_name, sex, weight, height, date_of_birth, email, password_hash) VALUES(?,?,?,?,?,?,?,?);", 
        {"Test", "User2", "male", 75, 185, "2002-02-02", "testuser2@mail.com", "password"}
    );

    query(userQuery, {}, userRes);
    query(profileQuery, {}, profileRes);
    query(scanQuery, {}, scanRes);

    // Read
    qDebug() << "\nPRINTING USERS ******************";
    for (int i = 0; i < userRes.size(); ++i) {
        qDebug() << "Row" << i + 1 << ":";
        const QMap<QString, QVariant>& row = userRes[i];
        for (auto it = row.begin(); it != row.end(); ++it) {
            qDebug() << "  " << it.key() << "=" << it.value();
        }
    }
    
    qDebug() << "\nPRINTING PROFILES ******************";
    for (int i = 0; i < profileRes.size(); ++i) {
        qDebug() << "Row" << i + 1 << ":";
        const QMap<QString, QVariant>& row = profileRes[i];
        for (auto it = row.begin(); it != row.end(); ++it) {
            qDebug() << "  " << it.key() << "=" << it.value();
        }
    }

    qDebug() << "\nPRINTING SCANS ******************";
    for (int i = 0; i < scanRes.size(); ++i) {
        qDebug() << "Row" << i + 1 << ":";
        const QMap<QString, QVariant>& row = scanRes[i];
        for (auto it = row.begin(); it != row.end(); ++it) {
            qDebug() << "  " << it.key() << "=" << it.value();
        }
    }

    // Update
    qDebug() << "\nUPDATING USERS ******************";
    execute(
        "UPDATE users SET first_name=?, last_name=?  WHERE email=?;", 
        {"Updated", "User2", "testuser2@mail.com"}
    );
    query(userQuery, {}, userRes);
    for (int i = 0; i < userRes.size(); ++i) {
        qDebug() << "Row" << i + 1 << ":";
        const QMap<QString, QVariant>& row = userRes[i];
        for (auto it = row.begin(); it != row.end(); ++it) {
            qDebug() << "  " << it.key() << "=" << it.value();
        }
    }

    // Delete
    qDebug() << "\nDELETING FROM USERS ******************";
    execute("DELETE FROM users WHERE email=?;", {"testuser2@mail.com"});
    query(userQuery, {}, userRes);
    for (int i = 0; i < userRes.size(); ++i) {
        qDebug() << "Row" << i + 1 << ":";
        const QMap<QString, QVariant>& row = userRes[i];
        for (auto it = row.begin(); it != row.end(); ++it) {
            qDebug() << "  " << it.key() << "=" << it.value();
        }
    }
}
