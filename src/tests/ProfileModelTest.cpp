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
    const QString sex = "male";
    int weight = 75;
    int height = 185;
    QDate dob(2000, 01, 01);

    ProfileModel prof(id, userId, name, sex, weight, height, dob);
    qDebug() << "\nTesting Profile Model";
    qDebug() << prof.toString();
    if ((prof.getId() == id) && (prof.getUserId() == userId) &&
        (prof.getName() == name) && (prof.getSex() == sex) &&
        (prof.getWeight() == weight) && (prof.getHeight() == height) &&
        (prof.getDob() == dob)

    ) {
        qDebug() << "All Tests Passed";
        return true;
    }

    qDebug() << "Testing failed";
    return false;
}
