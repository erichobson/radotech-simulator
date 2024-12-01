/**
 * @file HealthMetricCalculatorTest.h
 * @brief Declaration of the HealthMetricCalculatorTest class.
 */

#ifndef HEALTH_METRIC_CALCULATOR_TEST_H
#define HEALTH_METRIC_CALCULATOR_TEST_H

#include "Test.h"
#include "HealthMetricModel.h"
#include "HealthMetricCalculator.h"
#include "ScanModel.h"
#include <QVector>
#include <QDate>
#include <QDebug>

class HealthMetricCalculatorTest : public Test {
public:
    HealthMetricCalculatorTest();
    ~HealthMetricCalculatorTest();
    virtual bool test() const override;
};

#endif
