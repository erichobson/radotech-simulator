/**
 * @file ProfileModelTest.h
 * @brief Declaration of the ProfileModelTest class.
 */

#ifndef PROFILE_MODEL_TEST_H 
#define PROFILE_MODEL_TEST_H 

#include "Test.h"
#include "ProfileModel.h"
#include <QDebug>
#include <QString>

class ProfileModelTest : public Test {

public: 
    ProfileModelTest();
    ~ProfileModelTest();
    virtual bool test () const override;
};

#endif

