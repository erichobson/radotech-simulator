/**
 * @file UserController.h
 * @brief Declaration of the UserController class.
 */

#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "DatabaseManager.h"
#include "UserModel.h"
#include "ProfileModel.h"
#include <QVector>
#include <QDate>
#include <QString>
#include <QMap>
#include <QVariant>
#include <QList>

class UserController {

signals:
    void createProfile(int, const QString&, const QString&, const QString&, int, int, const QDate&);

public:
    UserController(DatabaseManager&);
    ~UserController();
    bool createUser(const QString &, const QString &,const QString &, int, int, const QDate &,const QString &, const QString &);
    bool createUser(UserModel*);
    bool getUserById(int, UserModel&) const;
    bool updateUser(int, const QString &, const QString &,const QString &, const QString &);
    bool updateUser(UserModel*);
    bool getUserProfiles(int, QVector<ProfileModel*>&) const;
    int getUserId(const QString &);

private:
    DatabaseManager& db;
};

#endif

