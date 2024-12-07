/**
 * @file ScanModel.cpp
 * @brief A model of the scan.
 */

#include "ScanModel.h"

#include <QDebug>

const QVector<QString> ScanModel::organNames = {
    "Lung",          "Heart Constrictor", "Heart",        "Small Intestine",
    "Triple Heater", "Large Intestine",   "Spleen",       "Liver",
    "Kidney",        "Urinary Bladder",   "Gall Bladder", "Stomach"};

ScanModel::ScanModel() {}

ScanModel::ScanModel(int id, int profileId,

                     int h1Lung, int h1LungR,

                     int h2HeartConstrictor, int h2HeartConstrictorR,

                     int h3Heart, int h3HeartR,

                     int h4SmallIntestine, int h4SmallIntestineR,

                     int h5TripleHeater, int h5TripleHeaterR,

                     int h6LargeIntestine, int h6LargeIntestineR,

                     int f1Spleen, int f1SpleenR,

                     int f2Liver, int f2LiverR,

                     int f3Kidney, int f3KidneyR,

                     int f4UrinaryBladder, int f4UrinaryBladderR,

                     int f5GallBladder, int f5GallBladderR,

                     int f6Stomach, int f6StomachR,

                     const QDate& createdOn,

                     // optional fields
                     int bodyTemp, int bloodPressure, int heartRate,
                     int sleepingTime, int currentWeight, int emotionalState,
                     int overallFeeling, const QString& name,
                     const QString& notes)
    : id(id),
      profileId(profileId),

      h1Lung(h1Lung),
      h1LungR(h1LungR),
      h2HeartConstrictor(h2HeartConstrictor),
      h2HeartConstrictorR(h2HeartConstrictorR),
      h3Heart(h3Heart),
      h3HeartR(h3HeartR),
      h4SmallIntestine(h4SmallIntestine),
      h4SmallIntestineR(h4SmallIntestineR),
      h5TripleHeater(h5TripleHeater),
      h5TripleHeaterR(h5TripleHeaterR),
      h6LargeIntestine(h6LargeIntestine),
      h6LargeIntestineR(h6LargeIntestineR),

      f1Spleen(f1Spleen),
      f1SpleenR(f1SpleenR),
      f2Liver(f2Liver),
      f2LiverR(f2LiverR),
      f3Kidney(f3Kidney),
      f3KidneyR(f3KidneyR),
      f4UrinaryBladder(f4UrinaryBladder),
      f4UrinaryBladderR(f4UrinaryBladderR),
      f5GallBladder(f5GallBladder),
      f5GallBladderR(f5GallBladderR),
      f6Stomach(f6Stomach),
      f6StomachR(f6StomachR),
      createdOn(createdOn),

      // optional fields
      bodyTemp(bodyTemp),
      bloodPressure(bloodPressure),
      heartRate(heartRate),
      sleepingTime(sleepingTime),
      currentWeight(currentWeight),
      emotionalState(emotionalState),
      overallFeeling(overallFeeling),
      name(name),
      notes(notes) {
    measurements = {h1Lung,
                    h1LungR,
                    h2HeartConstrictor,
                    h2HeartConstrictorR,
                    h3Heart,
                    h3HeartR,
                    h4SmallIntestine,
                    h4SmallIntestineR,
                    h5TripleHeater,
                    h5TripleHeaterR,
                    h6LargeIntestine,
                    h6LargeIntestineR,
                    f1Spleen,
                    f1SpleenR,
                    f2Liver,
                    f2LiverR,
                    f3Kidney,
                    f3KidneyR,
                    f4UrinaryBladder,
                    f4UrinaryBladderR,
                    f5GallBladder,
                    f5GallBladderR,
                    f6Stomach,
                    f6StomachR};

    upperMeasurements = {
        h1Lung,          h2HeartConstrictor,
        h3Heart,         h4SmallIntestine,
        h5TripleHeater,  h6LargeIntestine,
        h1LungR,         h2HeartConstrictorR,
        h3HeartR,        h4SmallIntestineR,
        h5TripleHeaterR, h6LargeIntestineR,
    };

    lowerMeasurements = {f1Spleen,          f2Liver,        f3Kidney,
                         f4UrinaryBladder,  f5GallBladder,  f6Stomach,
                         f1SpleenR,         f2LiverR,       f3KidneyR,
                         f4UrinaryBladderR, f5GallBladderR, f6StomachR};

    rightMeasurements = {
        h1LungR,         h2HeartConstrictorR, h3HeartR,       h4SmallIntestineR,
        h5TripleHeaterR, h6LargeIntestineR,   f1SpleenR,      f2LiverR,
        f3KidneyR,       f4UrinaryBladderR,   f5GallBladderR, f6StomachR};

    leftMeasurements = {
        h1Lung,         h2HeartConstrictor, h3Heart,       h4SmallIntestine,
        h5TripleHeater, h6LargeIntestine,   f1Spleen,      f2Liver,
        f3Kidney,       f4UrinaryBladder,   f5GallBladder, f6Stomach};
}

ScanModel::~ScanModel() {}

int ScanModel::getId() const { return id; }

void ScanModel::setId(int id) { this->id = id; }

int ScanModel::getProfileId() const { return profileId; }

void ScanModel::setProfileId(int id) { this->profileId = id; }

// Meridian Points (Left)
int ScanModel::getH1Lung() const { return h1Lung; }

void ScanModel::setH1Lung(int h1Lung) { this->h1Lung = h1Lung; }

int ScanModel::getH2HeartConstrictor() const { return h2HeartConstrictor; }

void ScanModel::setH2HeartConstrictor(int h2HeartConstrictor) {
    this->h2HeartConstrictor = h2HeartConstrictor;
}

int ScanModel::getH3Heart() const { return h3Heart; }

void ScanModel::setH3Heart(int h3Heart) { this->h3Heart = h3Heart; }

int ScanModel::getH4SmallIntestine() const { return h4SmallIntestine; }

void ScanModel::setH4SmallIntestine(int h4SmallIntestine) {
    this->h4SmallIntestine = h4SmallIntestine;
}

int ScanModel::getH5TripleHeater() const { return h5TripleHeater; }

void ScanModel::setH5TripleHeater(int h5TripleHeater) {
    this->h5TripleHeater = h5TripleHeater;
}

int ScanModel::getH6LargeIntestine() const { return h6LargeIntestine; }

void ScanModel::setH6LargeIntestine(int h6LargeIntestine) {
    this->h6LargeIntestine = h6LargeIntestine;
}

int ScanModel::getF1Spleen() const { return f1Spleen; }

void ScanModel::setF1Spleen(int f1Spleen) { this->f1Spleen = f1Spleen; }

int ScanModel::getF2Liver() const { return f2Liver; }

void ScanModel::setF2Liver(int f2Liver) { this->f2Liver = f2Liver; }

int ScanModel::getF3Kidney() const { return f3Kidney; }

void ScanModel::setF3Kidney(int f3Kidney) { this->f3Kidney = f3Kidney; }

int ScanModel::getF4UrinaryBladder() const { return f4UrinaryBladder; }

void ScanModel::setF4UrinaryBladder(int f4UrinaryBladder) {
    this->f4UrinaryBladder = f4UrinaryBladder;
}

int ScanModel::getF5GallBladder() const { return f5GallBladder; }

void ScanModel::setF5GallBladder(int f5GallBladder) {
    this->f5GallBladder = f5GallBladder;
}

int ScanModel::getF6Stomach() const { return f6Stomach; }

void ScanModel::setF6Stomach(int f6Stomach) { this->f6Stomach = f6Stomach; }

// Meridian Points (Right)
int ScanModel::getH1LungR() const { return h1LungR; }

void ScanModel::setH1LungR(int h1LungR) { this->h1LungR = h1LungR; }

int ScanModel::getH2HeartConstrictorR() const { return h2HeartConstrictorR; }

void ScanModel::setH2HeartConstrictorR(int h2HeartConstrictorR) {
    this->h2HeartConstrictorR = h2HeartConstrictorR;
}

int ScanModel::getH3HeartR() const { return h3HeartR; }

void ScanModel::setH3HeartR(int h3HeartR) { this->h3HeartR = h3HeartR; }

int ScanModel::getH4SmallIntestineR() const { return h4SmallIntestineR; }

void ScanModel::setH4SmallIntestineR(int h4SmallIntestineR) {
    this->h4SmallIntestineR = h4SmallIntestineR;
}

int ScanModel::getH5TripleHeaterR() const { return h5TripleHeaterR; }

void ScanModel::setH5TripleHeaterR(int h5TripleHeaterR) {
    this->h5TripleHeaterR = h5TripleHeaterR;
}

int ScanModel::getH6LargeIntestineR() const { return h6LargeIntestineR; }

void ScanModel::setH6LargeIntestineR(int h6LargeIntestineR) {
    this->h6LargeIntestineR = h6LargeIntestineR;
}

int ScanModel::getF1SpleenR() const { return f1SpleenR; }

void ScanModel::setF1SpleenR(int f1SpleenR) { this->f1SpleenR = f1SpleenR; }

int ScanModel::getF2LiverR() const { return f2LiverR; }

void ScanModel::setF2LiverR(int f2LiverR) { this->f2LiverR = f2LiverR; }

int ScanModel::getF3KidneyR() const { return f3KidneyR; }

void ScanModel::setF3KidneyR(int f3KidneyR) { this->f3KidneyR = f3KidneyR; }

int ScanModel::getF4UrinaryBladderR() const { return f4UrinaryBladderR; }

void ScanModel::setF4UrinaryBladderR(int f4UrinaryBladderR) {
    this->f4UrinaryBladderR = f4UrinaryBladderR;
}

int ScanModel::getF5GallBladderR() const { return f5GallBladderR; }

void ScanModel::setF5GallBladderR(int f5GallBladderR) {
    this->f5GallBladderR = f5GallBladderR;
}

int ScanModel::getF6StomachR() const { return f6StomachR; }

void ScanModel::setF6StomachR(int f6StomachR) { this->f6StomachR = f6StomachR; }

int ScanModel::getBodyTemp() const { return bodyTemp; }

void ScanModel::setBodyTemp(int bodyTemp) { this->bodyTemp = bodyTemp; }

int ScanModel::getBloodPressure() const { return bloodPressure; }

void ScanModel::setBloodPressure(int bloodPressure) {
    this->bloodPressure = bloodPressure;
}

int ScanModel::getHeartRate() const { return heartRate; }

void ScanModel::setHeartRate(int heartRate) { this->heartRate = heartRate; }

int ScanModel::getSleepingTime() const { return sleepingTime; }

void ScanModel::setSleepingTime(int sleepingTime) {
    this->sleepingTime = sleepingTime;
}

int ScanModel::getCurrentWeight() const { return currentWeight; }

void ScanModel::setCurrentWeight(int currentWeight) {
    this->currentWeight = currentWeight;
}

int ScanModel::getEmotionalState() const { return emotionalState; }

void ScanModel::setEmotionalState(int emotionalState) {
    this->emotionalState = emotionalState;
}

int ScanModel::getOverallFeeling() const { return overallFeeling; }

void ScanModel::setOverallFeeling(int overallFeeling) {
    this->overallFeeling = overallFeeling;
}

QString ScanModel::getName() const { return name; }

void ScanModel::setName(const QString& name) { this->name = name; }

QString ScanModel::getNotes() const { return notes; }

void ScanModel::setNotes(const QString& notes) { this->notes = notes; }

QDate ScanModel::getCreatedOn() const { return createdOn; }

void ScanModel::setCreatedOn(const QDate& createdOn) {
    this->createdOn = createdOn;
}

const QVector<int>& ScanModel::getMeasurements() const { return measurements; }

void ScanModel::setMeasurements(const QVector<int>& newMeasurements) {
    measurements = newMeasurements;

    // Update the grouped measurements
    if (measurements.size() >= 24) {
        upperMeasurements = {
            measurements[0],  measurements[1],
            measurements[2],  // h1-h3 Left
            measurements[3],  measurements[4],
            measurements[5],  // h4-h6 Left
            measurements[12], measurements[13],
            measurements[14],  // h1-h3 Right
            measurements[15], measurements[16],
            measurements[17]  // h4-h6 Right
        };

        lowerMeasurements = {
            measurements[6],  measurements[7],
            measurements[8],  // f1-f3 Left
            measurements[9],  measurements[10],
            measurements[11],  // f4-f6 Left
            measurements[18], measurements[19],
            measurements[20],  // f1-f3 Right
            measurements[21], measurements[22],
            measurements[23]  // f4-f6 Right
        };

        leftMeasurements = {
            measurements[0], measurements[1],  measurements[2],  // h1-h3 Left
            measurements[3], measurements[4],  measurements[5],  // h4-h6 Left
            measurements[6], measurements[7],  measurements[8],  // f1-f3 Left
            measurements[9], measurements[10], measurements[11]  // f4-f6 Left
        };

        rightMeasurements = {
            measurements[12], measurements[13],
            measurements[14],  // h1-h3 Right
            measurements[15], measurements[16],
            measurements[17],  // h4-h6 Right
            measurements[18], measurements[19],
            measurements[20],  // f1-f3 Right
            measurements[21], measurements[22],
            measurements[23]  // f4-f6 Right
        };
    }
}

const QVector<int>& ScanModel::getUpperMeasurements() const {
    return upperMeasurements;
}

const QVector<int>& ScanModel::getLowerMeasurements() const {
    return lowerMeasurements;
}

const QVector<int>& ScanModel::getRightMeasurements() const {
    return rightMeasurements;
}

const QVector<int>& ScanModel::getLeftMeasurements() const {
    return leftMeasurements;
}

const QVector<QString>& ScanModel::getOrganNames() { return organNames; }

QString ScanModel::toString() const {
    return QString(
               "ID: %1\n"
               "Name: %2\n"
               "Lung Health (H1 L): %3\n"
               "Lung Health (H1 R): %4\n"
               "Heart Constrictor (H2 L): %5\n"
               "Heart Constrictor (H2 R): %6n"
               "Heart Health (H3 L): %7\n"
               "Heart Health (H3 R): %8\n"
               "Small Intestine (H4 L): %9\n"
               "Small Intestine (H4 R): %10\n"
               "Triple Heater (H5 L): %11\n"
               "Triple Heater (H5 R): %12\n"
               "Large Intestine (H6 L): %13\n"
               "Large Intestine (H6 R): %14\n"
               "Spleen (F1 L): %15\n"
               "Spleen (F1 R): %16\n"
               "Liver (F2 L): %17\n"
               "Liver (F2 R): %18\n"
               "Kidney (F3 L): %19\n"
               "Kidney (F3 R): %20\n"
               "Urinary Bladder (F4 L): %21\n"
               "Urinary Bladder (F4 R): %22\n"
               "Gall Bladder (F5 L): %23\n"
               "Gall Bladder (F5 R): %24\n"
               "Stomach (F6 L): %25\n"
               "Stomach (F6 R): %26\n"
               "Body Temperature: %27\n"
               "Blood Pressure: %28\n"
               "Heart Rate: %29\n"
               "Sleeping Time: %30\n"
               "Current Weight: %31\n"
               "Emotional State: %32\n"
               "Overall Feeling: %33\n"
               "Notes: %34")
        .arg(id)
        .arg(name)
        .arg(h1Lung)
        .arg(h1LungR)
        .arg(h2HeartConstrictor)
        .arg(h2HeartConstrictorR)
        .arg(h3Heart)
        .arg(h3HeartR)
        .arg(h4SmallIntestine)
        .arg(h4SmallIntestineR)
        .arg(h5TripleHeater)
        .arg(h5TripleHeaterR)
        .arg(h6LargeIntestine)
        .arg(h6LargeIntestineR)
        .arg(f1Spleen)
        .arg(f1SpleenR)
        .arg(f2Liver)
        .arg(f2LiverR)
        .arg(f3Kidney)
        .arg(f3KidneyR)
        .arg(f4UrinaryBladder)
        .arg(f4UrinaryBladderR)
        .arg(f5GallBladder)
        .arg(f5GallBladderR)
        .arg(f6Stomach)
        .arg(f6StomachR)
        .arg(bodyTemp)
        .arg(bloodPressure)
        .arg(heartRate)
        .arg(sleepingTime)
        .arg(currentWeight)
        .arg(emotionalState)
        .arg(overallFeeling)
        .arg(notes);
}
