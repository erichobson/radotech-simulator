/**
 * @file HealthMetricCalculator.cpp
 * @brief Utility class that provides health metric calculations.
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
    energyLevelRange(25.0f, 55.0f),
    psychoStateRange(0.8f, 1.2f),
    skeletalSysRange(0.9f, 1.2f),
    immuneSysRange(47.0f, 57.0f),
    metabolismRange(1.1f, 1.2f)
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

    if(measurements.isEmpty() || measurements.size() <= 1) {
        qCritical() << "Error: Not enough measurements. Cannot calculate mean or standard deviation.";
        return false;
    }

    // Calcualate indicator metrics
    float energy = calculateEnergyHealth(scan);
    float psycho = calculatePsychoHealth(scan);
    float skeletal = calculateSkeletalHealth(scan);
    float metabolism = calculateMetabolismHealth(scan);
    float immune = calculateImmuneHealth(scan);
    
    if(energy == -1 || metabolism == -1 || immune == -1 || skeletal == -1 || psycho == -1) { // one of the calculations invalid
        qCritical() << "Error: could not calculate indicators";
        return false;
    }

    // Determine outliers and update hms
    int energyOutlier = energyLevelRange.withinRange(energy);
    int psychoOutlier = psychoStateRange.withinRange(psycho);
    int skeletalOutlier = skeletalSysRange.withinRange(skeletal);
    int metabolismOutlier = metabolismRange.withinRange(metabolism);
    int immuneOutlier = immuneSysRange.withinRange(immune);

    hms.clear();
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

    return true;
}


/**
 * @brief Calculates the trends from the provided vector of scans.
 * @param scan input parameter of the scans to calculate trends for. Expects scans to be in order of oldest to newest.
 * @param hms output parameter used to store the calculated indicator health metrics. 
     * Each health metric returns the trend name, the latest indicator value from the most recent scan, the trend type, and an integer representing trending up (+1), trending down(-1), or trending sideways(0).
 * @returns true if the calculations were successfull, false otherwise.
 */
bool HealthMetricCalculator::calculateTrendHealth(const QVector<ScanModel*>& scans, QVector<HealthMetricModel*>& hms) {

    if(scans.size() <= 1) {
        qCritical() << "Error: Not enough scans. Cannot calculate scan trends.";
        return false;
    }

    // get all the energy, metabolism, immune, skeletal, and psycho state indicators 
    QVector<float> energys;
    QVector<float> metabolisms;
    QVector<float> immunes;
    QVector<float> skeletals;
    QVector<float> psychos;
    
    // get all indicators for each scan
    for(int i = 0; i < scans.size(); ++i) { 
        ScanModel* scan = scans[i];

        energys.append(calculateEnergyHealth(scan));
        metabolisms.append(calculateMetabolismHealth(scan));
        immunes.append(calculateImmuneHealth(scan));
        skeletals.append(calculateSkeletalHealth(scan));
        psychos.append(calculatePsychoHealth(scan));

        if(energys[i] == -1 || metabolisms[i] == -1 || immunes[i] == -1 || skeletals[i] == -1 || psychos[i] == -1) { // one of the calculations invalid
            qCritical() << QString("Error: Scan: %1 (id = %2) does not have enough information to calculate trends").arg(scan->getName()).arg(scan->getId());
            return false;
        }
    }

    float newestEnergy = energys[energys.size() - 1];
    float newestMetabolism = metabolisms[metabolisms.size() - 1];
    float newestImmune = immunes[immunes.size() - 1];
    float newestSkeletal = skeletals[skeletals.size() - 1];
    float newestPsycho = psychos[psychos.size() - 1];

    // find the slope of the line of best fit for each one
    float energySlope = (newestEnergy - energys[0]) / (energys.size() - 1);
    float metabolismSlope = (newestMetabolism - metabolisms[0]) / (metabolisms.size() - 1);
    float immuneSlope = (newestImmune - immunes[0]) / (immunes.size() - 1);
    float skeletalSlope = (newestSkeletal - skeletals[0]) / (skeletals.size() - 1);
    float psychoSlope = (newestPsycho - psychos[0]) / (psychos.size() - 1);

    // update hms with the trend analysis for each of the indicators
    hms.clear();
    hms.append({
        new HealthMetricModel(
            "Energy level trend", 
            newestEnergy, 
            energySlope > 0 ? "Trending Up" : (energySlope < 0 ? "Trending Down" : "Trending Sideways"),
            energySlope > 0 ? 1 : (energySlope < 0 ? -1 : 0)
        ),
        new HealthMetricModel(
            "Immune system trend", 
            newestImmune, 
            immuneSlope > 0 ? "Trending Up" : (immuneSlope < 0 ? "Trending Down" : "Trending Sideways"),
            immuneSlope > 0 ? 1 : (immuneSlope < 0 ? -1 : 0)
        ),
        new HealthMetricModel(
            "Metabolism trend", 
            newestMetabolism, 
            metabolismSlope > 0 ? "Trending Up" : (metabolismSlope < 0 ? "Trending Down" : "Trending Sideways"),
            metabolismSlope > 0 ? 1 : (metabolismSlope< 0 ? -1 : 0)
        ),
        new HealthMetricModel(
            "Psycho-emotional state trend", 
            newestPsycho, 
            psychoSlope > 0 ? "Trending Up" : (psychoSlope < 0 ? "Trending Down" : "Trending Sideways"),
            psychoSlope > 0 ? 1 : (psychoSlope < 0 ? -1 : 0)
        ),
        new HealthMetricModel(
            "Musculoskeletal system trend", 
            newestSkeletal, 
            skeletalSlope > 0 ? "Trending Up" : (skeletalSlope < 0 ? "Trending Down" : "Trending Sideways"),
            skeletalSlope  > 0 ? 1 : (skeletalSlope < 0 ? -1 : 0)
        )
    });

    return true;
}

float HealthMetricCalculator::calculateEnergyHealth(ScanModel* scan) {

    const QVector<int>& measurements = scan->getMeasurements();

    if(measurements.size() <= 0) {
        qCritical() << "Error: not enough measurements to calculate energy indicator";
        return -1;
    }

    float running = 0;
    for(const auto& x : measurements) {
        running += x;
    }
    // Energy: mean
    return running / measurements.size();
}

float HealthMetricCalculator::calculateImmuneHealth(ScanModel* scan) {

    const QVector<int>& upperMeasurements = scan->getUpperMeasurements();
    const QVector<int>& lowerMeasurements = scan->getLowerMeasurements();

    if(upperMeasurements.size() <= 0 || lowerMeasurements.size() <= 0) {
        qCritical() << "Error: not enough measurements to calculate immune indicator";
        return -1;
    }

    float upperSum = 0;
    float lowerSum = 0;
    for(auto& x : upperMeasurements) {
        upperSum += x;
    }
    for(auto& x : lowerMeasurements) {
        lowerSum += x;
    }

    // Immune system: |upper-lower| / 2
    return std::abs(upperSum - lowerSum) / 2;
}

float HealthMetricCalculator::calculateMetabolismHealth(ScanModel* scan) {

    const QVector<int>& leftMeasurements = scan->getLeftMeasurements();
    const QVector<int>& rightMeasurements = scan->getRightMeasurements();

    if(rightMeasurements.size() <= 0 || leftMeasurements.size() <= 0) {
        qCritical() << "Error: not enough measurements to calculate metabolism indicator";
        return -1;
    }

    float leftWeightedSum = 0.0f;
    for(const auto& measurement : leftMeasurements) {
        float weight = random(0.0f, 0.5f);
        leftWeightedSum += measurement + (measurement * weight);
    }
    
    float rightWeightedSum = 0.0f;
    for(const auto& measurement : rightMeasurements) {
        float weight = random(0.0f, 0.5f);
        rightWeightedSum += measurement + (measurement * weight);
    }

    // Metabolism: left(weighted) / right(weighted)
    return std::abs(leftWeightedSum / rightWeightedSum); 
}

float HealthMetricCalculator::calculateSkeletalHealth(ScanModel* scan) {

    const QVector<int>& rightMeasurements = scan->getRightMeasurements();
    const QVector<int>& leftMeasurements = scan->getLeftMeasurements();

    if(rightMeasurements.size() <= 0 || leftMeasurements.size() <= 0) {
        qCritical() << "Error: not enough measurements to calculate skeletal indicator";
        return -1;
    }

    float leftSum = 0;
    float rightSum = 0;
    for(const auto& x : leftMeasurements) {
        leftSum += x;
    }
    for(const auto& x : rightMeasurements) {
        rightSum += x;
    }

    // Musculoskeletal system: left/right
    return std::abs(leftSum / rightSum); 
}

float HealthMetricCalculator::calculatePsychoHealth(ScanModel* scan) {

    const QVector<int>& upperMeasurements = scan->getUpperMeasurements();
    const QVector<int>& lowerMeasurements = scan->getLowerMeasurements();

    if(upperMeasurements.size() <= 0 || lowerMeasurements.size() <= 0) {
        qCritical() << "Error: not enough measurements to calculate psycho-emotional indicator";
        return -1;
    }

    float upperSum = 0;
    float lowerSum = 0;
    for(auto& x : upperMeasurements) {
        upperSum += x;
    }
    for(auto& x : lowerMeasurements) {
        lowerSum += x;
    }
    
    // Psycho-emotional state: upper/lower 
    return std::abs(upperSum / lowerSum);
}

float HealthMetricCalculator::random(float lower, float upper) {
    std::random_device rdev;
    std::default_random_engine re(rdev());
    std::uniform_real_distribution<> dist(lower, upper);
    return dist(re);
}
