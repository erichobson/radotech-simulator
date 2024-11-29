/**
 * @file src/utils/HealthMetricCalculator.cpp
 * @brief Utility class that provides health metric calculations
 */

#include "HealthMetricCalculator.h"

int Range::withinRange(float val) const {
    if(val > max) return 1;
    if(val < min) return -1;
    return 0;
}

/**
 * @brief Constructor for the HealthMetricCalculator class.
 */
HealthMetricCalculator::HealthMetricCalculator():
    energyLevelRange(25, 55),
    psychoStateRange(0.8, 1.2),
    skeletalSysRange(0.9, 1.2),
    immuneSysRange(47, 57),
    metabolismRange(1.1, 1.2)
{}

/**
 * @brief Destructor for the HealthMetricCalculator class.
 */
HealthMetricCalculator::~HealthMetricCalculator() {}

/**
 * @brief Calculates the organ health for each point in the scan. Compares them to the mean, using +- 20% as outlier detection 
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

    const QVector<QString>& organNames = ScanModel::getOrganNames();

    int j = 0;
    for(int i = 0; i < measurements.size(); ++i) {
        float left = measurements[i];
        float right = measurements[++i];
        float avg = (left + right) / 2;
        float upperBound = mean + (mean * .20);
        float lowerBound = mean - (mean * .20);

        HealthMetricModel* hm = nullptr;

        if(avg > upperBound) { // pos outlier
            hm = new HealthMetricModel(organNames[j], avg, "Organ above normal", 1);
        } else if(avg < lowerBound) { // neg outlier
            hm = new HealthMetricModel(organNames[j], avg, "Organ below normal", -1);
        } else { // normal
            hm = new HealthMetricModel(organNames[j], avg, "Organ is normal", 0);
        }

        hms.append(hm);
        ++j;
    }

    for (auto* hm : hms) {
        qDebug () << hm->toString();
    }

    return true;
}

/**
 * @brief Calculates the indicator health using various points in the scan.
 * @param scan input parameter used to calculate the indicator health.
 * @param hms output parameter used to store the calculated indicator health metrics
 * @returns true if the calculations were successfull, false otherwise
 */
bool HealthMetricCalculator::calculateIndicatorHealth(ScanModel* scan, QVector<HealthMetricModel*>& hms) {
    
    const QVector<int>& measurements = scan->getMeasurements();
    const QVector<int>& upperMeasurements = scan->getUpperMeasurements();
    const QVector<int>& lowerMeasurements = scan->getLowerMeasurements();
    const QVector<int>& rightMeasurements = scan->getRightMeasurements();
    const QVector<int>& leftMeasurements = scan->getLeftMeasurements();

    if(measurements.isEmpty() || measurements.size() <= 1) {
        qCritical() << "Error: Not enough measurements. Cannot calculate mean or standard deviation.";
        return false;
    }

    // Energy: mean
    float running = 0;
    for(const auto& x : measurements) {
        running += x;
    }
    float energy = running / measurements.size();

    // Psycho-emotional state: upper/lower 
    float upperSum = 0;
    float lowerSum = 0;
    for(auto& x : upperMeasurements) {
        upperSum += x;
    }
    for(auto& x : lowerMeasurements) {
        lowerSum += x;
    }
    float psycho = upperSum / lowerSum;

    // Musculoskeletal system: left/right
    float leftSum = 0;
    float rightSum = 0;
    for(const auto& x : leftMeasurements) {
        leftSum += x;
    }
    for(const auto& x : rightMeasurements) {
        rightSum += x;
    }
    float skeletal = leftSum / rightSum;

    // Metabolism: left(weighted) / right(weighted)
    QVector<int> lweights;
    QVector<int> rweights;
    lweights.clear();
    rweights.clear();
    for(const auto& _ : leftMeasurements) {
        lweights.append(random(0.0, 0.5));
    }
    for(const auto& _ : rightMeasurements) {
        rweights.append(random(0.0, 0.5));
    }    
    
    float leftWeightedSum = 0;
    float rightWeightedSum = 0;
    for(int i = 0; i < leftMeasurements.size(); ++i) {
        leftWeightedSum += leftMeasurements[i] + (leftMeasurements[i] * lweights[i]);
    }
    for(int i = 0; i < rightMeasurements.size(); ++i) {
        rightWeightedSum += rightMeasurements[i] + (rightMeasurements[i] * rweights[i]);
    }
    float metabolism = leftWeightedSum / rightWeightedSum;

    // Immune system: |upper-lower| / 2
    float immune = std::abs(upperSum - lowerSum) / 2;

    hms.clear();
    // Determine outliers and update hms
    int energyOutlier = energyLevelRange.withinRange(energy);
    int psychoOutlier = psychoStateRange.withinRange(psycho);
    int skeletalOutlier = skeletalSysRange.withinRange(skeletal);
    int metabolismOutlier = metabolismRange.withinRange(metabolism);
    int immuneOutlier = immuneSysRange.withinRange(immune);

    hms.append({
        new HealthMetricModel(
            "Energy Level", 
            energy, 
            energyOutlier > 0 ? "Above Normal" : (energyOutlier < 0 ? "Below Normal" : "Normal"),
            energyOutlier
        ),
        new HealthMetricModel(
            "Immune system", 
            immune, 
            immuneOutlier > 0 ? "Above Normal" : (immuneOutlier < 0 ? "Below Normal" : "Normal"),
            immuneOutlier 
        ),
        new HealthMetricModel(
            "Metabolism", 
            metabolism, 
            metabolismOutlier > 0 ? "Above Normal" : (metabolismOutlier < 0 ? "Below Normal" : "Normal"),
            metabolismOutlier 
        ),
        new HealthMetricModel(
            "Psycho-emotional state", 
            psycho, 
            psychoOutlier > 0 ? "Above Normal" : (psychoOutlier < 0 ? "Below Normal" : "Normal"),
            psychoOutlier
        ),
        new HealthMetricModel(
            "Musculoskeletal system", 
            skeletal, 
            skeletalOutlier > 0 ? "Above Normal" : (skeletalOutlier < 0 ? "Below Normal" : "Normal"),
            skeletalOutlier
        )
    });
    
    for (auto* hm : hms) {
        qDebug () << hm->toString();
    }

    return true;
}


float HealthMetricCalculator::random(float lower, float upper) {
    std::random_device rdev;
    std::default_random_engine re(rdev());
    std::uniform_real_distribution<> dist(lower, upper);
    return dist(re);
}
