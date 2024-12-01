/**
 * @file ScanModelTest.h
 * @brief Declaration of the ScanModelTest class.
 */

#ifndef SCAN_MODEL_TEST_H 
#define SCAN_MODEL_TEST_H 

#include "Test.h"
#include "ScanModel.h"
#include <QDebug>
#include <QDate>
#include <QString>

class ScanModelTest : public Test {

public: 
    ScanModelTest();
    ~ScanModelTest();
    virtual bool test () const override;
};

#endif

