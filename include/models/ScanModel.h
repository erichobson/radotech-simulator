/**
 * @file include/models/ScanModel.h
 * @brief A model of the scan.
 */

#ifndef SCAN_MODEL_H
#define SCAN_MODEL_H

#include <QString>
#include <QDate>
#include <QVector>
class ScanModel {

    public:
        ScanModel();
        ScanModel(
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
            // opt
            int bodyTemp = -1, 
            int bloodPressure = -1, 
            int heartRate = -1, 
            int sleepingTime = -1,
            int currentWeight = -1,
            int emotionalState = -1, 
            int overallFeeling = -1, 
            const QString& name = "", 
            const QString& notes = ""
        ); 

        ~ScanModel();

        int getId() const;
        void setId(int id);
        
        int getProfileId() const;
        void setProfileId(int id);

        QString getName() const;
        void setName(const QString& name);

        int getH1Lung() const;
        void setH1Lung(int h1Lung);

        int getH2HeartConstrictor() const;
        void setH2HeartConstrictor(int h2HeartConstrictor);

        int getH3Heart() const;
        void setH3Heart(int h3Heart);

        int getH4SmallIntestine() const;
        void setH4SmallIntestine(int h4SmallIntestine);

        int getH5TripleHeater() const;
        void setH5TripleHeater(int h5TripleHeater);

        int getH6LargeIntestine() const;
        void setH6LargeIntestine(int h6LargeIntestine);

        int getF1Spleen() const;
        void setF1Spleen(int f1Spleen);

        int getF2Liver() const;
        void setF2Liver(int f2Liver);

        int getF3Kidney() const;
        void setF3Kidney(int f3Kidney);

        int getF4UrinaryBladder() const;
        void setF4UrinaryBladder(int f4UrinaryBladder);

        int getF5GallBladder() const;
        void setF5GallBladder(int f5GallBladder);

        int getF6Stomach() const;
        void setF6Stomach(int f6Stomach);

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

        QString getNotes() const;
        void setNotes(const QString& notes);
       
        QString toString() const;

        const QVector<int>& getMeasurements() const;
        void setMeasurements(const QVector<int>&);

        void test();

    private:
        int id;
        int profileId;
        int h1Lung;
        int h2HeartConstrictor;
        int h3Heart;
        int h4SmallIntestine;
        int h5TripleHeater;
        int h6LargeIntestine;
        int f1Spleen;
        int f2Liver;
        int f3Kidney;
        int f4UrinaryBladder;
        int f5GallBladder;
        int f6Stomach;
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
};

#endif
