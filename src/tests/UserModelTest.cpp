/**
 * @file UserModelTest.cpp
 * @brief Tests for the UserModel class.
 */

#include "UserModelTest.h"

UserModelTest::UserModelTest() {}
UserModelTest::~UserModelTest() {}

bool UserModelTest::test() const {
    int id = 0;
    const QString fName = "Andrew";
    const QString lName = "Wallace";
    const QString email = "andrew@mail.com";
    const QString pass = "pass";

    UserModel user(id, fName, lName, email, pass);

    qDebug() << "\nTesting User";
    qDebug() << user.toString();
    if ((user.getId() == id) && (user.getFirstName() == fName) &&
        (user.getLastName() == lName) && (user.getEmail() == email) &&
        (user.getPasswordHash() == pass)) {
        qDebug() << "All tests passed";
        return true;
    }

    qDebug() << "Test Failed";
    return false;
}
