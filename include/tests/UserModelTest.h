/**
 * @file UserModelTest.h
 * @brief Declaration of the UserModelTest class.
 */

#ifndef USER_MODEL_TEST_H 
#define USER_MODEL_TEST_H 

#include "Test.h"
#include "UserModel.h"
#include <QDebug>
#include <QString>
#include <QDate>

class UserModelTest : public Test {

public: 
    UserModelTest();
    ~UserModelTest();
    virtual bool test () const override;
};

#endif

