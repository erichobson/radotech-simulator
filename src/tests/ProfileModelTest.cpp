/**
 * @file ProfileModelTest.cpp
 * @brief Tests for the ProfileModel class.
 */

#include "ProfileModelTest.h"

ProfileModelTest::ProfileModelTest() {}
ProfileModelTest::~ProfileModelTest() {}

bool ProfileModelTest::test() const {

    int id = -1;
    int userId = 3;
    QString name = "Test Profile";
    QString desc = "Test desc";

    ProfileModel prof(id, userId, name, desc);
    qDebug() << "\nTesting Profile Model";
    qDebug() << prof.toString();
    if(
        (prof.getId() == id) &&
        (prof.getUserId() == userId) &&
        (prof.getName() == name) &&
        (prof.getDesc() == desc)
    ) {
        qDebug() << "All Tests Passed";
        return true;
    }

    qDebug() << "Testing failed";
    return false;
}

