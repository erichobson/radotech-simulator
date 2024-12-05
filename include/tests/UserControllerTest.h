
/**
 * @file UserControllerTest.h
 * @brief Declaration of the UserControllerTest class.
 */

#ifndef USER_CONTROLLER_TEST_H
#define USER_CONTROLLER_TEST_H

#include "Test.h"
#include "UserController.h"
#include "ProfileModel.h"
#include "DatabaseManager.h"
#include <QDebug>

class UserControllerTest : public Test { 
public:
    UserControllerTest(DatabaseManager&);
    ~UserControllerTest();
    virtual bool test () const override;
private:
    bool testCRUD() const;
    void cleanup(QVector<ProfileModel*>&) const;
    UserController* uc;
};
#endif

