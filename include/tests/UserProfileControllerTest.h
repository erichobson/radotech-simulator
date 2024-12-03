/**
 * @file UserProfileControllerTest.h
 * @brief Declaration of the UserProfileControllerTest class.
 */

#ifndef USER_PROFILE_CONTROLLER_TEST_H
#define USER_PROFILE_CONTROLLER_TEST_H

#include "Test.h"
#include "UserProfileController.h"
#include "DatabaseManager.h"
#include <QDebug>

class UserProfileControllerTest : public Test { 
public:
    UserProfileControllerTest(DatabaseManager&);
    ~UserProfileControllerTest();
    virtual bool test () const override;
private:
    bool testCRUD() const;
    void cleanup(QVector<ProfileModel*>&) const;
    UserProfileController* upc;
};
#endif

