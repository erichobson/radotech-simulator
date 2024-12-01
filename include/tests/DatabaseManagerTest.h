/**
 * @file DatabaseManagerTest.h
 * @brief Declaration of the DatabaseManagerTest class.
 */

#ifndef DATABASE_MANAGER_TEST_H 
#define DATABASE_MANAGER_TEST_H 

#include "Test.h"
#include "DatabaseManager.h"
#include <QDebug>

class DatabaseManagerTest : public Test {

public: 
    DatabaseManagerTest();
    ~DatabaseManagerTest();
    virtual bool test () const override;

private:
    bool testCRUD() const;
};

#endif



