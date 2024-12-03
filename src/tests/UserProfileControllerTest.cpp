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
    if(!(upc->createProfile(1, "Test Profile x", "Test Profile x Description"))){
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

    if(!upc->updateProfile(profile.getId(), profile.getUserId(), "Test Profile x Updated", "Test Profile x Description Updated")){
        cleanup(profiles);
        return false;
    }


    if(!(upc->getProfileByName(1, "Test Profile x Updated", profile))){
        qDebug() << "Could not find profile: " << "Test Profile x";
        cleanup(profiles);
        return false;
    } 
    qDebug() << profile.toString();


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

