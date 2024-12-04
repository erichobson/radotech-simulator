/**
 * @file DatabaseManagerTest.cpp
 * @brief Tests for the DatabaseManager class.
 */

#include "DatabaseManagerTest.h"

DatabaseManagerTest::DatabaseManagerTest(DatabaseManager& db) : db(db) {}
DatabaseManagerTest::~DatabaseManagerTest() {}

bool DatabaseManagerTest::test() const { return testCRUD(); }

bool DatabaseManagerTest::testCRUD() const {
    const QString userQuery = "SELECT * FROM users;";
    const QString profileQuery = "SELECT * FROM profile;";
    const QString scanQuery = "SELECT * FROM scan;";

    QList<QMap<QString, QVariant>> userRes;
    QList<QMap<QString, QVariant>> profileRes;
    QList<QMap<QString, QVariant>> scanRes;

    // Create
    qDebug() << "\nCREATING USER " << "Test User 2" << " ******************";
    // db.execute("INSERT INTO users (first_name, last_name, sex, weight,
    // height, date_of_birth, email, password_hash) VALUES(?,?,?,?,?,?,?,?);",
    //     {"Test", "User2", "male", 75, 185, "2002-02-02",
    //     "testuser2@mail.com", "password"}
    // );
    db.execute(
        "INSERT INTO users (first_name, last_name, email, password_hash) "
        "VALUES(?,?,?,?);",
        {"Test", "User2", "male", "testuser2@mail.com", "password"});

    db.query(userQuery, {}, userRes);
    db.query(profileQuery, {}, profileRes);
    db.query(scanQuery, {}, scanRes);

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
    db.execute("UPDATE users SET first_name=?, last_name=?  WHERE email=?;",
               {"Updated", "User2", "testuser2@mail.com"});
    db.query(userQuery, {}, userRes);
    for (int i = 0; i < userRes.size(); ++i) {
        qDebug() << "Row" << i + 1 << ":";
        const QMap<QString, QVariant>& row = userRes[i];
        for (auto it = row.begin(); it != row.end(); ++it) {
            qDebug() << "  " << it.key() << "=" << it.value();
        }
    }

    // Delete
    qDebug() << "\nDELETING FROM USERS ******************";
    db.execute("DELETE FROM users WHERE email=?;", {"testuser2@mail.com"});
    db.query(userQuery, {}, userRes);
    for (int i = 0; i < userRes.size(); ++i) {
        qDebug() << "Row" << i + 1 << ":";
        const QMap<QString, QVariant>& row = userRes[i];
        for (auto it = row.begin(); it != row.end(); ++it) {
            qDebug() << "  " << it.key() << "=" << it.value();
        }
    }

    return true;
}
