/**
 * @file src/models/ScanModel.cpp
 * @brief A model of the scan.
 */

#include "ScanModel.h"
#include <QDebug>

const QVector<QString> ScanModel::organNames = {
    "Lung", "Heart Constrictor", "Heart", "Small Intestine", "Triple Heater", 
    "Large Intestine", "Spleen", "Liver", "Kidney", "Urinary Bladder", 
    "Gall Bladder", "Stomach"
};

ScanModel::ScanModel() {}

ScanModel::ScanModel(
    int id, 
    int profileId, 
    int h1Lung, 
    int h2HeartConstrictor, 
    int h3Heart, 
    int h4SmallIntestine, 
    int h5TripleHeater, 
    int h6LargeIntestine, 
    int f1Spleen, 
    int f2Liver, 
    int f3Kidney, 
    int f4UrinaryBladder, 
    int f5GallBladder, 
    int f6Stomach,

    int h1LungR, 
    int h2HeartConstrictorR, 
    int h3HeartR, 
    int h4SmallIntestineR, 
    int h5TripleHeaterR, 
    int h6LargeIntestineR, 
    int f1SpleenR, 
    int f2LiverR, 
    int f3KidneyR, 
    int f4UrinaryBladderR, 
    int f5GallBladderR, 
    int f6StomachR,

    int bodyTemp, 
    int bloodPressure, 
    int heartRate, 
    int sleepingTime, 
    int currentWeight,
    int emotionalState, 
    int overallFeeling, 
    const QString& name, 
    const QString& notes) : 
    id(id), 
    profileId(profileId), 

    h1Lung(h1Lung), 
    h2HeartConstrictor(h2HeartConstrictor), 
    h3Heart(h3Heart),
    h4SmallIntestine(h4SmallIntestine), 
    h5TripleHeater(h5TripleHeater), 
    h6LargeIntestine(h6LargeIntestine),
    f1Spleen(f1Spleen), 
    f2Liver(f2Liver), 
    f3Kidney(f3Kidney), 
    f4UrinaryBladder(f4UrinaryBladder),
    f5GallBladder(f5GallBladder), 
    f6Stomach(f6Stomach),

    h1LungR(h1LungR), 
    h2HeartConstrictorR(h2HeartConstrictorR), 
    h3HeartR(h3HeartR),
    h4SmallIntestineR(h4SmallIntestineR), 
    h5TripleHeaterR(h5TripleHeaterR), 
    h6LargeIntestineR(h6LargeIntestineR),
    f1SpleenR(f1SpleenR), 
    f2LiverR(f2LiverR), 
    f3KidneyR(f3KidneyR), 
    f4UrinaryBladderR(f4UrinaryBladderR)    
    f5GallBladderR(f5GallBladderR), 
    f6StomachR(f6StomachR),

    bodyTemp(bodyTemp), 
    bloodPressure(bloodPressure),
    heartRate(heartRate), 
    sleepingTime(sleepingTime), 
    currentWeight(currentWeight), 
    emotionalState(emotionalState),
    overallFeeling(overallFeeling), 
    name(name),
    notes(notes) 
{
    measurements = {
        h1Lung, h2HeartConstrictor, h3Heart, h4SmallIntestine, h5TripleHeater, h6LargeIntestine, 
        f1Spleen, f2Liver, f3Kidney, f4UrinaryBladder, f5GallBladder, f6Stomach
        h1LungR, h2HeartConstrictorR, h3HeartR, h4SmallIntestineR, h5TripleHeaterR, h6LargeIntestineR, 
        f1SpleenR, f2LiverR, f3KidneyR, f4UrinaryBladderR, f5GallBladderR, f6StomachR
    };
}

ScanModel::~ScanModel() {}

int ScanModel::getId() const {
    return id;
}

void ScanModel::setId(int id) {
    this->id = id;
}

int ScanModel::getProfileId() const {
    return profileId;
}

void ScanModel::setProfileId(int id) {
    this->profileId = id;
}

// Meridian Points (Left)
int ScanModel::getH1Lung() const {
    return h1Lung;
}

void ScanModel::setH1Lung(int h1Lung) {
    this->h1Lung = h1Lung;
}

int ScanModel::getH2HeartConstrictor() const {
    return h2HeartConstrictor;
}

void ScanModel::setH2HeartConstrictor(int h2HeartConstrictor) {
    this->h2HeartConstrictor = h2HeartConstrictor;
}

int ScanModel::getH3Heart() const {
    return h3Heart;
}

void ScanModel::setH3Heart(int h3Heart) {
    this->h3Heart = h3Heart;
}

int ScanModel::getH4SmallIntestine() const {
    return h4SmallIntestine;
}

void ScanModel::setH4SmallIntestine(int h4SmallIntestine) {
    this->h4SmallIntestine = h4SmallIntestine;
}

int ScanModel::getH5TripleHeater() const {
    return h5TripleHeater;
}

void ScanModel::setH5TripleHeater(int h5TripleHeater) {
    this->h5TripleHeater = h5TripleHeater;
}

int ScanModel::getH6LargeIntestine() const {
    return h6LargeIntestine;
}

void ScanModel::setH6LargeIntestine(int h6LargeIntestine) {
    this->h6LargeIntestine = h6LargeIntestine;
}

int ScanModel::getF1Spleen() const {
    return f1Spleen;
}

void ScanModel::setF1Spleen(int f1Spleen) {
    this->f1Spleen = f1Spleen;
}

int ScanModel::getF2Liver() const {
    return f2Liver;
}

void ScanModel::setF2Liver(int f2Liver) {
    this->f2Liver = f2Liver;
}

int ScanModel::getF3Kidney() const {
    return f3Kidney;
}

void ScanModel::setF3Kidney(int f3Kidney) {
    this->f3Kidney = f3Kidney;
}

int ScanModel::getF4UrinaryBladder() const {
    return f4UrinaryBladder;
}

void ScanModel::setF4UrinaryBladder(int f4UrinaryBladder) {
    this->f4UrinaryBladder = f4UrinaryBladder;
}

int ScanModel::getF5GallBladder() const {
    return f5GallBladder;
}

void ScanModel::setF5GallBladder(int f5GallBladder) {
    this->f5GallBladder = f5GallBladder;
}

int ScanModel::getF6Stomach() const {
    return f6Stomach;
}

void ScanModel::setF6Stomach(int f6Stomach) {
    this->f6Stomach = f6Stomach;
}

// Meridian Points (Right)
int ScanModel::getH1LungR() const {
    return h1LungR;
}

void ScanModel::setH1LungR(int h1LungR) {
    this->h1LungR = h1LungR;
}

int ScanModel::getH2HeartConstrictorR() const {
    return h2HeartConstrictorR;
}

void ScanModel::setH2HeartConstrictorR(int h2HeartConstrictorR) {
    this->h2HeartConstrictorR = h2HeartConstrictorR;
}

int ScanModel::getH3HeartR() const {
    return h3HeartR;
}

void ScanModel::setH3HeartR(int h3HeartR) {
    this->h3HeartR = h3HeartR;
}

int ScanModel::getH4SmallIntestineR() const {
    return h4SmallIntestineR;
}

void ScanModel::setH4SmallIntestineR(int h4SmallIntestineR) {
    this->h4SmallIntestineR = h4SmallIntestineR;
}

int ScanModel::getH5TripleHeaterR() const {
    return h5TripleHeaterR;
}

void ScanModel::setH5TripleHeaterR(int h5TripleHeaterR) {
    this->h5TripleHeaterR = h5TripleHeaterR;
}

int ScanModel::getH6LargeIntestineR() const {
    return h6LargeIntestineR;
}

void ScanModel::setH6LargeIntestineR(int h6LargeIntestineR) {
    this->h6LargeIntestineR = h6LargeIntestineR;
}

int ScanModel::getF1SpleenR() const {
    return f1SpleenR;
}

void ScanModel::setF1SpleenR(int f1SpleenR) {
    this->f1SpleenR = f1SpleenR;
}

int ScanModel::getF2LiverR() const {
    return f2LiverR;
}

void ScanModel::setF2LiverR(int f2LiverR) {
    this->f2LiverR = f2LiverR;
}

int ScanModel::getF3KidneyR() const {
    return f3KidneyR;
}

void ScanModel::setF3KidneyR(int f3KidneyR) {
    this->f3KidneyR = f3KidneyR;
}

int ScanModel::getF4UrinaryBladderR() const {
    return f4UrinaryBladderR;
}

void ScanModel::setF4UrinaryBladderR(int f4UrinaryBladderR) {
    this->f4UrinaryBladderR = f4UrinaryBladderR;
}

int ScanModel::getF5GallBladderR() const {
    return f5GallBladderR;
}

void ScanModel::setF5GallBladderR(int f5GallBladderR) {
    this->f5GallBladderR = f5GallBladderR;
}

int ScanModel::getF6StomachR() const {
    return f6StomachR;
}

void ScanModel::setF6StomachR(int f6StomachR) {
    this->f6StomachR = f6StomachR;
}

int ScanModel::getBodyTemp() const {
    return bodyTemp;
}

void ScanModel::setBodyTemp(int bodyTemp) {
    this->bodyTemp = bodyTemp;
}

int ScanModel::getBloodPressure() const {
    return bloodPressure;
}

void ScanModel::setBloodPressure(int bloodPressure) {
    this->bloodPressure = bloodPressure;
}

int ScanModel::getHeartRate() const {
    return heartRate;
}

void ScanModel::setHeartRate(int heartRate) {
    this->heartRate = heartRate;
}

int ScanModel::getSleepingTime() const {
    return sleepingTime;
}

void ScanModel::setSleepingTime(int sleepingTime) {
    this->sleepingTime = sleepingTime;
}

int ScanModel::getCurrentWeight() const {
    return currentWeight;
}

void ScanModel::setCurrentWeight(int currentWeight) {
    this->currentWeight = currentWeight;
}

int ScanModel::getEmotionalState() const {
    return emotionalState;
}

void ScanModel::setEmotionalState(int emotionalState) {
    this->emotionalState = emotionalState;
}

int ScanModel::getOverallFeeling() const {
    return overallFeeling;
}

void ScanModel::setOverallFeeling(int overallFeeling) {
    this->overallFeeling = overallFeeling;
}

QString ScanModel::getName() const {
    return name;
}

void ScanModel::setName(const QString& name) {
    this->name = name;
}

QString ScanModel::getNotes() const {
    return notes;
}

void ScanModel::setNotes(const QString& notes) {
    this->notes = notes;
}

const QVector<int>& ScanModel::getMeasurements() const {
    return measurements;
}

void ScanModel::setMeasurements(const QVector<int>& newMeasurements) {
    measurements = newMeasurements;
}

const QVector<QString>& ScanModel::getOrganNames() {
    return organNames;
}

QString ScanModel::toString() const {
    return QString("ID: %1\n"
                   "Name: %2\n"
                   "Lung Health (H1): %3\n"
                   "Heart Constrictor (H2): %4\n"
                   "Heart Health (H3): %5\n"
                   "Small Intestine (H4): %6\n"
                   "Triple Heater (H5): %7\n"
                   "Large Intestine (H6): %8\n"
                   "Spleen (F1): %9\n"
                   "Liver (F2): %10\n"
                   "Kidney (F3): %11\n"
                   "Urinary Bladder (F4): %12\n"
                   "Gall Bladder (F5): %13\n"
                   "Stomach (F6): %14\n"
                   "Body Temperature: %15\n"
                   "Blood Pressure: %16\n"
                   "Heart Rate: %17\n"
                   "Sleeping Time: %18\n"
                   "Current Weight: %19\n"
                   "Emotional State: %20\n"
                   "Overall Feeling: %21\n"
                   "Notes: %22")
            .arg(id)
            .arg(name)
            .arg(h1Lung)
            .arg(h2HeartConstrictor)
            .arg(h3Heart)
            .arg(h4SmallIntestine)
            .arg(h5TripleHeater)
            .arg(h6LargeIntestine)
            .arg(f1Spleen)
            .arg(f2Liver)
            .arg(f3Kidney)
            .arg(f4UrinaryBladder)
            .arg(f5GallBladder)
            .arg(f6Stomach)
            .arg(bodyTemp)
            .arg(bloodPressure)
            .arg(heartRate)
            .arg(sleepingTime)
            .arg(currentWeight)
            .arg(emotionalState)
            .arg(overallFeeling)
            .arg(notes);
}



void ScanModel::test() {
    int id = -1;
    int profileId = 2;

    int h1 = 155;
    int h2 = 156;
    int h3 = 157;
    int h4 = 158;
    int h5 = 159;
    int h6 = 155;
    
    int f1 = 155;
    int f2 = 22;
    int f3 = -157;
    int f4 = 158;
    int f5 = 200;
    int f6 = 2000;

    int bt = 37;
    int bp = 22;
    int hr = 188;
    int st = 2232;
    int cw = 185;
    int em = 1;
    int ofeel = 2;
    QString name = "Andrew scan";
    QString notes = "Test note";

    ScanModel scan(id,profileId,h1,h2,h3,h4,h5,h6,f1,f2,f3,f4,f5,f6,bt,bp,hr,st,cw,em,ofeel,name,notes);

    qDebug() << "\nTesting Scan";
    qDebug() << scan.toString();
    if(
        (scan.getId() == id) &&
        (scan.getProfileId() == profileId) &&

        (scan.getH1Lung() == h1) &&
        (scan.getH2HeartConstrictor() == h2) &&
        (scan.getH3Heart() == h3) &&
        (scan.getH4SmallIntestine() == h4) &&
        (scan.getH5TripleHeater() == h5) &&
        (scan.getH6LargeIntestine() == h6) &&

        (scan.getF1Spleen() == f1) &&
        (scan.getF2Liver() == f2) &&
        (scan.getF3Kidney() == f3) &&
        (scan.getF4UrinaryBladder() == f4) &&
        (scan.getF5GallBladder() == f5) &&
        (scan.getF6Stomach() == f6) &&

        (scan.getBodyTemp() == bt) &&
        (scan.getBloodPressure() == bp) &&
        (scan.getHeartRate() == hr) &&
        (scan.getSleepingTime() == st) &&
        (scan.getCurrentWeight() == cw) &&
        (scan.getEmotionalState() == em) &&
        (scan.getOverallFeeling() == ofeel) &&
        (scan.getName() == name) &&
        (scan.getNotes() == notes)
    ) {
        qDebug() << "All Tests Passed";
        qDebug() << "Printing measurements";
        for(int m : scan.getMeasurements()) qDebug() << QString("%1").arg(m);
    } else{
        qDebug() << "Tests Failed";
    }

}
