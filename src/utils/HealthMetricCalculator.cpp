/**
 * @file src/utils/HealthMetricCalculator.cpp
 * @brief Utility class that provides health metric calculations
 */

#include "HealthMetricCalculator.h"

/**
 * @brief Constructor for the HealthMetricCalculator class.
 */
HealthMetricCalculator::HealthMetricCalculator() {}

/**
 * @brief Destructor for the HealthMetricCalculator class.
 */
HealthMetricCalculator::~HealthMetricCalculator() {}

/**
 * @brief Calculates the organ health for each point in the scan. Compares them to the mean, using z-index as outlier detection.
 * @param scan input parameter used to calculate the organ health.
 * @param hms output parameter used to store the calculated organ health metrics
 * @returns true if the calculations were successfull, false otherwise
 */
bool HealthMetricCalculator::calculateOrganHealth(ScanModel* scan, QVector<HealthMetricModel*>& hms) {
    const QVector<int>& measurements = scan->getMeasurements();
    hms.clear();

    if(measurements.isEmpty() || measurements.size() <= 1) {
        qCritical() << "Error: Not enough measurements. Cannot calculate mean or standard deviation.";
        return false;
    }

    // Calculate mean
    float running = 0;
    for(auto& x : measurements) {
        running += x;
    }
    float mean = running / measurements.size();

    // Calculate sample std deviation
    float squaredDifferencesSum = 0;
    for(auto& x : measurements) {
        squaredDifferencesSum += std::pow(x - mean, 2);
    }
    float stdDeviation = std::sqrt(squaredDifferencesSum / (measurements.size() - 1));

    // Calculate z-score for each point and determine outliers
    const QVector<QString>& organNames = ScanModel::getOrganNames();

    for(int i = 0; i < measurements.size(); ++i) {
        float x = measurements[i];
        float zScore = 0;
        
        if (stdDeviation != 0) {
            zScore = (x - mean) / stdDeviation;
        }

        HealthMetricModel* hm = nullptr;

        if(zScore > 2) { // pos outlier
            hm = new HealthMetricModel(organNames[i], x, "Organ above normal", 1);
        } else if(zScore < -2) { // neg outlier
            hm = new HealthMetricModel(organNames[i], x, "Organ below normal", -1);
        } else { // normal
            hm = new HealthMetricModel(organNames[i], x, "Organ is normal", 0);
        }

        hms.append(hm);
    }

    for (auto* hm : hms) {
        qDebug () << hm->toString();
    }

    return true;
}