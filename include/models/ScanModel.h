/**
 * @file ScanModel.h
 * @brief A model of the scan.
 */

#ifndef SCAN_MODEL_H
#define SCAN_MODEL_H

#include <QDate>
#include <QString>
#include <QVector>
class ScanModel {
   public:
    ScanModel();
    ScanModel(int id, int profileId, int h1Lung, int h1LungR,
              int h2HeartConstrictor, int h2HeartConstrictorR, int h3Heart,
              int h3HeartR, int h4SmallIntestine, int h4SmallIntestineR,
              int h5TripleHeater, int h5TripleHeaterR, int h6LargeIntestine,
              int h6LargeIntestineR, int f1Spleen, int f1SpleenR, int f2Liver,
              int f2LiverR, int f3Kidney, int f3KidneyR, int f4UrinaryBladder,
              int f4UrinaryBladderR, int f5GallBladder, int f5GallBladderR,
              int f6Stomach, int f6StomachR,

              const QDate& createdOn,
              // opt
              int bodyTemp = -1, int bloodPressure = -1, int heartRate = -1,
              int sleepingTime = -1, int currentWeight = -1,
              int emotionalState = -1, int overallFeeling = -1,
              const QString& name = "", const QString& notes = "");

    ~ScanModel();

    int getId() const;
    void setId(int id);

    int getProfileId() const;
    void setProfileId(int id);

    QString getName() const;
    void setName(const QString& name);

    // Meridian Points (Left)
    int getH1Lung() const;
    void setH1Lung(int);

    int getH2HeartConstrictor() const;
    void setH2HeartConstrictor(int);

    int getH3Heart() const;
    void setH3Heart(int);

    int getH4SmallIntestine() const;
    void setH4SmallIntestine(int);

    int getH5TripleHeater() const;
    void setH5TripleHeater(int);

    int getH6LargeIntestine() const;
    void setH6LargeIntestine(int);

    int getF1Spleen() const;
    void setF1Spleen(int);

    int getF2Liver() const;
    void setF2Liver(int);

    int getF3Kidney() const;
    void setF3Kidney(int);

    int getF4UrinaryBladder() const;
    void setF4UrinaryBladder(int);

    int getF5GallBladder() const;
    void setF5GallBladder(int);

    int getF6Stomach() const;
    void setF6Stomach(int);

    // Meridian Points (Right)
    int getH1LungR() const;
    void setH1LungR(int);

    int getH2HeartConstrictorR() const;
    void setH2HeartConstrictorR(int);

    int getH3HeartR() const;
    void setH3HeartR(int);

    int getH4SmallIntestineR() const;
    void setH4SmallIntestineR(int);

    int getH5TripleHeaterR() const;
    void setH5TripleHeaterR(int);

    int getH6LargeIntestineR() const;
    void setH6LargeIntestineR(int);

    int getF1SpleenR() const;
    void setF1SpleenR(int);

    int getF2LiverR() const;
    void setF2LiverR(int);

    int getF3KidneyR() const;
    void setF3KidneyR(int);

    int getF4UrinaryBladderR() const;
    void setF4UrinaryBladderR(int);

    int getF5GallBladderR() const;
    void setF5GallBladderR(int);

    int getF6StomachR() const;
    void setF6StomachR(int);

    int getBodyTemp() const;
    void setBodyTemp(int bodyTemp);

    int getBloodPressure() const;
    void setBloodPressure(int bloodPressure);

    int getHeartRate() const;
    void setHeartRate(int heartRate);

    int getSleepingTime() const;
    void setSleepingTime(int sleepingTime);

    int getCurrentWeight() const;
    void setCurrentWeight(int currentWeight);

    int getEmotionalState() const;
    void setEmotionalState(int emotionalState);

    int getOverallFeeling() const;
    void setOverallFeeling(int overallFeeling);

    QDate getCreatedOn() const;
    void setCreatedOn(const QDate&);

    QString getNotes() const;
    void setNotes(const QString& notes);

    QString toString() const;

    const QVector<int>& getMeasurements() const;
    void setMeasurements(const QVector<int>&);

    const QVector<int>& getUpperMeasurements() const;
    const QVector<int>& getLowerMeasurements() const;

    const QVector<int>& getRightMeasurements() const;
    const QVector<int>& getLeftMeasurements() const;

    static const QVector<QString>& getOrganNames();

   private:
    int id;
    int profileId;

    int h1Lung;
    int h1LungR;
    int h2HeartConstrictor;
    int h2HeartConstrictorR;
    int h3Heart;
    int h3HeartR;
    int h4SmallIntestine;
    int h4SmallIntestineR;
    int h5TripleHeater;
    int h5TripleHeaterR;
    int h6LargeIntestine;
    int h6LargeIntestineR;
    int f1Spleen;
    int f1SpleenR;
    int f2Liver;
    int f2LiverR;
    int f3Kidney;
    int f3KidneyR;
    int f4UrinaryBladder;
    int f4UrinaryBladderR;
    int f5GallBladder;
    int f5GallBladderR;
    int f6Stomach;
    int f6StomachR;
    QDate createdOn;

    int bodyTemp;
    int bloodPressure;
    int heartRate;
    int sleepingTime;
    int currentWeight;
    int emotionalState;
    int overallFeeling;
    QString name;
    QString notes;

    QVector<int> measurements;
    QVector<int> upperMeasurements;
    QVector<int> lowerMeasurements;
    QVector<int> rightMeasurements;
    QVector<int> leftMeasurements;
    static const QVector<QString> organNames;
};

#endif
