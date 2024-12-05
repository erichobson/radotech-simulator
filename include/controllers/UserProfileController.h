/**
 * @file UserProfileController.h
 * @brief Declaration of the UserProfileController class.
 */

#ifndef USER_PROFILE_CONTROLLER_H
#define USER_PROFILE_CONTROLLER_H

#include "DatabaseManager.h"
#include "ProfileModel.h"
#include "ScanModel.h"
#include <QVector>
#include <QDate>
#include <QString>
#include <QMap>
#include <QVariant>
#include <QList>

class UserProfileController {

private slots:
    void onCreateProfile(int, const QString&, const QString&, const QString&, int, int, const QDate&);

public:
    UserProfileController(DatabaseManager&);
    ~UserProfileController();
    bool getProfileByName(int, const QString&, ProfileModel&) const;
    bool getProfiles(int, QVector<ProfileModel*>&) const;
    bool createProfile(int, const QString&, const QString&, const QString&, int, int, const QDate&);
    bool createProfile(ProfileModel*);
    bool updateProfile(int, int, const QString&, const QString&, const QString&, int, int, const QDate&);
    bool updateProfile(ProfileModel*);
    bool deleteProfile(int);
    bool deleteProfile(ProfileModel*);
    bool getProfileScans(int, QVector<ScanModel*>&) const;

private:
    DatabaseManager& db;
};

#endif

