/**
 * @file UserProfileControllerTest.cpp
 * @brief Tests for the UserProfileController class.
 */

#include "UserProfileControllerTest.h"

UserProfileControllerTest::UserProfileControllerTest(DatabaseManager& db) {
    upc = new UserProfileController(db);
}

UserProfileControllerTest::~UserProfileControllerTest() {
    delete upc;
}

bool UserProfileControllerTest::test() const {
    return testCRUD();
}

bool UserProfileControllerTest::testCRUD() const {

    QVector<ProfileModel*> profiles;
    ProfileModel profile;

    qDebug () << "\n\nTesting UserProfileController";
    // Create
    qDebug() << "\nCREATING PROFILE:" << "Test Profile x" <<" ******************";
    if(!(upc->createProfile(1, "Test Profile x", "Test Profile x Description", "Male", 55, 185, QDate(2024, 02, 02)))){
        cleanup(profiles);
        return false;
    }


    // Read
    qDebug() << "\nREADING PROFILES FOR USER_ID: 1 ******************";
    if(!(upc->getProfiles(1, profiles))){
        cleanup(profiles);
        return false; 
    } 

    for(const auto* prof : profiles) {
        qDebug() << prof->toString();
        delete prof;
    }

    // Update
    qDebug() << "\nUPDATING PROFILE: " << "Test Profile x" <<" ******************";
    if(!upc->getProfileByName(1, "Test Profile x", profile)){
        cleanup(profiles);
        return false;
    } 

    if(!upc->updateProfile(profile.getId(), profile.getUserId(), "Test Profile x Updated", "Test Profile x Description Updated", "female", 57, 175, QDate(1999, 03, 03))){
        cleanup(profiles);
        return false;
    }

    if(!(upc->getProfileByName(1, "Test Profile x Updated", profile))){
        qDebug() << "Could not find profile: " << "Test Profile x Updated";
        cleanup(profiles);
        return false;
    } 
    qDebug() << profile.toString();


    // Get profile scans
    qDebug() << "\nGETTING PROFILE SCANS FOR: " << "Test Profile 1" << "******************";
    if(!(upc->getProfileByName(1, "Test Profile 1", profile))){
        qDebug() << "Could not find profile: " << "Test Profile 1";
        cleanup(profiles);
        return false;
    }
    QVector<ScanModel*> scans;
    if(!(upc->getProfileScans(profile.getId(), scans))){
        qDebug() << "Could not get scans for: " << "Test Profile x Updated";
        cleanup(profiles);
        return false;
    }

    for(const auto* scan : scans) {
        qDebug() << scan->toString();
        delete scan;
    }


    // Delete
    qDebug() << "\nDELETING PROFILE: " << "Test Profile x Updated" << "******************";
    if(!upc->getProfileByName(1, "Test Profile x Updated", profile)){
        cleanup(profiles);
        return false;
    }

    if(!upc->deleteProfile(profile.getId())){
        cleanup(profiles);
        return false;
    } 

    if(!upc->getProfiles(1, profiles)) {
        cleanup(profiles);
        return false; 
    }

    for(const auto* prof : profiles) {
        qDebug() << prof->toString();
        delete prof;
    }


    return true;
}

void UserProfileControllerTest::cleanup(QVector<ProfileModel*>& profiles) const {
    for(const auto* prof : profiles) delete prof;
}

