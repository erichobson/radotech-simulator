/**
 * @file include/utils/DatabaseManager.h
 * @brief
 */

#include <QtSql>
class DatabaseManager {

    public: 
        DatabaseManager();
        ~DatabaseManager();


    private:
        QSqlDatabase dbConnection;

};
