
/**
 * @file UserControllerTest.cpp
 * @brief Tests for the UserController class.
 */

#include "UserControllerTest.h"
#include "ProfileModel.h"

UserControllerTest::UserControllerTest(DatabaseManager& db) {
    uc = new UserController(db);
}

UserControllerTest::~UserControllerTest() {
    delete uc;
}

bool UserControllerTest::test() const {
    return testCRUD();
}

bool UserControllerTest::testCRUD() const {

    QVector<ProfileModel*> profiles;
    UserModel user;

    QString fName = "John";
    QString lName = "Doe";
    QString email = "john@mail.com";
    QString password = "password";

    qDebug () << "\n\nTesting UserController";
    // Create
    qDebug() << "\nCREATING USER:" << "John Doe" <<" ******************";
    if(!(uc->createUser(fName, lName, email, password, user))){
        cleanup(profiles);
        return false;
    }

    // Read
    qDebug() << "\nVALIDTING USER: John Doe ******************";
    if(!(uc->validateUser(email, password, user))){
        cleanup(profiles);
        return false; 
    } 
    qDebug() << user.toString();

    // Update
    qDebug() << "\nUPDATING USER: John Doe to Mary Jane ******************";
    bool updateUser(int, const QString&, const QString&, const QString&, QString&, UserModel&);
    fName = "Mary";
    lName = "Jane";
    email = "mary@mail.com";
    password = "new_password";
    if(!uc->updateUser(user.getId(), fName, lName, email, password, user)){
        cleanup(profiles);
        return false;
    }

    // Read
    qDebug() << "\nVALIDTING USER: Mary Jane ******************";
    if(!(uc->validateUser(email, password, user))){
        cleanup(profiles);
        return false; 
    } 
    qDebug() << user.toString();

    // Get user profiles 
    qDebug() << "\nGETTING USER PROFILES FOR user_id: " << "1" << "******************";
    if(!(uc->getUserProfiles(1, profiles))){
        qDebug() << "Could not find profiles: " << "Test  1";
        cleanup(profiles);
        return false;
    }

    for(const auto* profile : profiles) {
        qDebug() << profile->toString();
        delete profile;
    }

    // Delete
    qDebug() << "\nDELETING USER: " << "Mary Jane" << "******************";
    if(!uc->deleteUser(user.getId())){
        cleanup(profiles);
        return false;
    }

    return true;
}

void UserControllerTest::cleanup(QVector<ProfileModel*>& profiles) const {
    for(const auto* prof : profiles) delete prof;
}

