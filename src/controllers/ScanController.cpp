/**
 * @file ScanController.cpp
 * @brief A model of the ScanController.
 */

#include "ScanController.h"
#include <QRandomGenerator>

ScanController::ScanController(DatabaseManager& db_) : db(db_) {}

void ScanController::createScan(const QVector<int>& measurements, ProfileModel& profile){
    ScanModel scan;

    scan.setProfileId(profile.getId());
    scan.setName(profile.getName());
    scan.setH1Lung(measurements[0]);
    scan.setH1LungR(measurements[1]);
    scan.setH2HeartConstrictor(measurements[2]);
    scan.setH2HeartConstrictorR(measurements[3]);
    scan.setH3Heart(measurements[4]);
    scan.setH3HeartR(measurements[5]);
    scan.setH4SmallIntestine(measurements[6]);
    scan.setH4SmallIntestineR(measurements[7]);
    scan.setH5TripleHeater(measurements[8]);
    scan.setH5TripleHeaterR(measurements[9]);
    scan.setH6LargeIntestine(measurements[10]);
    scan.setH6LargeIntestineR(measurements[11]);
    scan.setF1Spleen(measurements[12]);
    scan.setF1SpleenR(measurements[13]);
    scan.setF2Liver(measurements[14]);
    scan.setF2LiverR(measurements[15]);
    scan.setF3Kidney(measurements[16]);
    scan.setF3KidneyR(measurements[17]);
    scan.setF4UrinaryBladder(measurements[18]);
    scan.setF4UrinaryBladderR(measurements[19]);
    scan.setF5GallBladder(measurements[20]);
    scan.setF5GallBladderR(measurements[21]);
    scan.setF6Stomach(measurements[22]);
    scan.setF6StomachR(measurements[23]);
    scan.setBodyTemp(measurements[24]);
    scan.setBloodPressure(measurements[25]);
    scan.setHeartRate(measurements[26]);
    scan.setSleepingTime(measurements[27]);
    scan.setCurrentWeight(measurements[28]);
    scan.setEmotionalState(measurements[29]);
    scan.setOverallFeeling(measurements[30]);

    this->storeScan(scan);
}

int ScanController::generateMeasurement(int range_start, int range_end){
    return QRandomGenerator::global()->bounded(range_start, range_end);
}

bool ScanController::storeScan(ScanModel& scan) {
   try{
        db.execute("INSERT INTO scan VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)",
                  {
                       scan.getProfileId(),
                       scan.getName(),
                       scan.getH1Lung(),
                       scan.getH1LungR(),
                       scan.getH2HeartConstrictor(),
                       scan.getH2HeartConstrictorR(),
                       scan.getH3Heart(),
                       scan.getH3HeartR(),
                       scan.getH4SmallIntestine(),
                       scan.getH4SmallIntestineR(),
                       scan.getH5TripleHeater(),
                       scan.getH5TripleHeaterR(),
                       scan.getH6LargeIntestine(),
                       scan.getH6LargeIntestineR(),
                       scan.getF1Spleen(),
                       scan.getF1SpleenR(),
                       scan.getF2Liver(),
                       scan.getF2LiverR(),
                       scan.getF3Kidney(),
                       scan.getF3KidneyR(),
                       scan.getF4UrinaryBladder(),
                       scan.getF4UrinaryBladderR(),
                       scan.getF5GallBladder(),
                       scan.getF5GallBladderR(),
                       scan.getF6Stomach(),
                       scan.getF6StomachR(),
                       scan.getBodyTemp(),
                       scan.getBloodPressure(),
                       scan.getHeartRate(),
                       scan.getSleepingTime(),
                       scan.getCurrentWeight(),
                       scan.getEmotionalState(),
                       scan.getOverallFeeling(),
                   }
            );
        return true;
    } catch (const std::exception& e) {
        qCritical() << "Failed to upload scan: " << e.what();
        return false;
    }
}
