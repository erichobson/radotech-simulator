/**
 * @file include/utils/DatabaseManager.h
 * @brief
 */

#include <QtSql>
#include <QString>
#include <QList>
#include <QVariant>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DatabaseManager {

    public: 
        DatabaseManager();
        ~DatabaseManager();

        void init();
        void execute(const QString&, const QList<QVariant>&);
        void query(const QString&, const QList<QVariant>&, QList<QMap<QString, QVariant>>&);
        bool isConnectionOpen();

    private:
        QSqlDatabase dbConnection;
        void handleError(const QSqlError&);
        void executeSqlScript(const QString&, QSqlDatabase&);
        bool isDataPresent(const QList<QString>&);
};
