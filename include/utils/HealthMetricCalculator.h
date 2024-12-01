/**
 * @file HealthMetricCalculator.h
 * @brief Utility class providing health metric calculations.
 */

#ifndef HEALTH_METRIC_CALCULATOR_H
#define HEALTH_METRIC_CALCULATOR_H

#include <QVector>
#include <QString>
#include <QDebug>
#include <cmath>
#include <random>

#include "HealthMetricModel.h"
#include "ScanModel.h"

struct Range {
    float min;
    float max;
    Range(float min, float max): min(min), max(max){}
    int withinRange(float val) const;
};

class HealthMetricCalculator {

    public:
        HealthMetricCalculator();
        ~HealthMetricCalculator();
        bool calculateOrganHealth(ScanModel*, QVector<HealthMetricModel*>&);
        bool calculateIndicatorHealth(ScanModel*, QVector<HealthMetricModel*>&);
        bool calculateTrendHealth(const QVector<ScanModel*>&, QVector<HealthMetricModel*>&);

        float calculateEnergyHealth(ScanModel*);
        float calculateImmuneHealth(ScanModel*);
        float calculateSkeletalHealth(ScanModel*);
        float calculateMetabolismHealth(ScanModel*);
        float calculatePsychoHealth(ScanModel*); 

    private: 
        Range energyLevelRange{25.0f, 55.0f};
        Range psychoStateRange{0.8f, 1.2f};
        Range skeletalSysRange{0.9f, 1.2f};
        Range immuneSysRange{47.0f, 57.0f};
        Range metabolismRange{1.1f, 1.2f};

        float random(float, float);
};
#endif
