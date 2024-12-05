/**
 * @file UserController.h
 * @brief Declaration of the UserController class.
 */

#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "UserModel.h"
#include "DatabaseManager.h"
#include "ProfileModel.h"
#include <QString>
#include <QCryptographicHash>
#include <QByteArray>
#include <QList>
#include <QVariant>
#include <QVector>

class UserController {

public:
    UserController(DatabaseManager&);
    ~UserController();

    bool getUserByEmail(const QString&, UserModel&) const;
    bool getUserById(int, UserModel&) const;
    
    bool validateUser(const QString&, const QString&, UserModel& user);
    bool createUser(const QString&, const QString&, const QString&, const QString&, UserModel&);
    bool createUser(UserModel*);
    bool updateUser(int, const QString&, const QString&, const QString&, const QString&, UserModel&);
    bool updateUser(UserModel*);
    bool deleteUser(int);
    bool deleteUser(UserModel*);
    bool getUserProfiles(int, QVector<ProfileModel*>&) const;

private:
    DatabaseManager& db;
    QString hash(const QString&);

};

#endif

