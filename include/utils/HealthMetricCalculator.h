/**
 * @file include/utils/HealthMetricCalculator.h
 * @brief Utility class providing health metric calculations
 */

#ifndef HEALTH_METRIC_CALCULATOR_H
#define HEALTH_METRIC_CALCULATOR_H

#include <QVector>
#include <QString>
#include <QDebug>
#include <cmath>

#include "HealthMetricModel.h"
#include "ScanModel.h"

class HealthMetricCalculator {

    public:
        HealthMetricCalculator();
        ~HealthMetricCalculator();
        bool calculateOrganHealth(ScanModel*, QVector<HealthMetricModel*>&);
        //QVector<HealthMetricModel*> calculateIndicatorHealth(ScanModel*);
};
#endif
