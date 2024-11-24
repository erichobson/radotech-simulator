/**
 * @file src/models/ScanModel.cpp
 * @brief A model of the scan.
 */

#include "ScanModel.h"

ScanModel::ScanModel(
    int id, 
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
    int f6Stomac,
    int bodyTemp, 
    int bloodPressure, 
    int heartRate, 
    int sleepingTime, 
    int currentWeight,
    int emotionalState, 
    int overallFeeling, 
    const QString& name, 
    const QString& notes) : 
    id(id), name(name), h1Lung(h1Lung), h2HeartConstrictor(h2HeartConstrictor), h3Heart(h3Heart),
    h4SmallIntestine(h4SmallIntestine), h5TripleHeater(h5TripleHeater), h6LargeIntestine(h6LargeIntestine),
    f1Spleen(f1Spleen), f2Liver(f2Liver), f3Kidney(f3Kidney), f4UrinaryBladder(f4UrinaryBladder),
    f5GallBladder(f5GallBladder), f6Stomach(f6Stomach), bodyTemp(bodyTemp), bloodPressure(bloodPressure),
    heartRate(heartRate), sleepingTime(sleepingTime), currentWeight(currentWeight), emotionalState(emotionalState),
    overallFeeling(overallFeeling), notes(notes) 
{}

ScanModel::~ScanModel() {}

int ScanModel::getId() const {
    return id;
}

void ScanModel::setId(int id) {
    this->id = id;
}

QString ScanModel::getName() const {
    return name;
}

void ScanModel::setName(const QString& name) {
    this->name = name;
}

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

QString ScanModel::getNotes() const {
    return notes;
}

void ScanModel::setNotes(const QString& notes) {
    this->notes = notes;
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
