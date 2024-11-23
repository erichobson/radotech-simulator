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

    const QString userQuery = "SELECT * FROM users;";
    const QString profileQuery = "SELECT * FROM profile;";
    const QString scanQuery = "SELECT * FROM scan;";

    QList<QMap<QString, QVariant>> userRes;
    QList<QMap<QString, QVariant>> profileRes;
    QList<QMap<QString, QVariant>> scanRes;


    db.query(userQuery, {}, userRes);
    db.query(profileQuery, {}, profileRes);
    db.query(scanQuery, {}, scanRes);

    for (int i = 0; i < userRes.size(); ++i) {
        qDebug() << "Row" << i + 1 << ":";
        const QMap<QString, QVariant>& row = userRes[i];
        for (auto it = row.begin(); it != row.end(); ++it) {
            qDebug() << "  " << it.key() << "=" << it.value();
        }
    }
    
    for (int i = 0; i < profileRes.size(); ++i) {
        qDebug() << "Row" << i + 1 << ":";
        const QMap<QString, QVariant>& row = profileRes[i];
        for (auto it = row.begin(); it != row.end(); ++it) {
            qDebug() << "  " << it.key() << "=" << it.value();
        }
    }

    for (int i = 0; i < scanRes.size(); ++i) {
        qDebug() << "Row" << i + 1 << ":";
        const QMap<QString, QVariant>& row = scanRes[i];
        for (auto it = row.begin(); it != row.end(); ++it) {
            qDebug() << "  " << it.key() << "=" << it.value();
        }
    }

}
