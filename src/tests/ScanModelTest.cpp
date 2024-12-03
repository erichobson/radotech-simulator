/**
 * @file ScanModelTest.cpp
 * @brief Tests for the ScanModel class.
 */

#include "ScanModelTest.h"

ScanModelTest::ScanModelTest() {}
ScanModelTest::~ScanModelTest() {}

bool ScanModelTest::test() const {
    int id = -1;
    int profileId = 2;

    int h1 = 55;
    int h2 = 56;
    int h3 = 57;
    int h4 = 58;
    int h5 = 59;
    int h6 = 55;
    
    int h1r = 55;
    int h2r = 56;
    int h3r = 57;
    int h4r = 58;
    int h5r = 59;
    int h6r = 55;
    
    int f1 = 55;
    int f2 = 22;
    int f3 = -157;
    int f4 = 58;
    int f5 = 25;
    int f6 = 2000;
    
    int f1r = 75;
    int f2r = 52;
    int f3r = 57;
    int f4r = 58;
    int f5r = 25;
    int f6r = 200;

    int bt = 37;
    int bp = 22;
    int hr = 188;
    int st = 2232;
    int cw = 185;
    int em = 1;
    int ofeel = 2;
    QDate createdOn(2000, 06, 06);
    QString name = "Andrew scan";
    QString notes = "Test note";

    ScanModel scan(
        id,profileId,
        h1,h1r,h2,h2r,h3,h3r,h4,h4r,h5,h5r,h6,h6r,
        f1,f1r,f2,f2r,f3,f3r,f4,f4r,f5,f5r,f6,f6r,
        createdOn,
        bt,bp,hr,st,cw,em,ofeel,name,notes
    );

    qDebug() << "\nTesting Scan Model";
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

        (scan.getH1LungR() == h1r) &&
        (scan.getH2HeartConstrictorR() == h2r) &&
        (scan.getH3HeartR() == h3r) &&
        (scan.getH4SmallIntestineR() == h4r) &&
        (scan.getH5TripleHeaterR() == h5r) &&
        (scan.getH6LargeIntestineR() == h6r) &&

        (scan.getF1Spleen() == f1) &&
        (scan.getF2Liver() == f2) &&
        (scan.getF3Kidney() == f3) &&
        (scan.getF4UrinaryBladder() == f4) &&
        (scan.getF5GallBladder() == f5) &&
        (scan.getF6Stomach() == f6) &&

        (scan.getF1SpleenR() == f1r) &&
        (scan.getF2LiverR() == f2r) &&
        (scan.getF3KidneyR() == f3r) &&
        (scan.getF4UrinaryBladderR() == f4r) &&
        (scan.getF5GallBladderR() == f5r) &&
        (scan.getF6StomachR() == f6r) &&

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
        return true;
    } 

    qDebug() << "Tests Failed";
    return false;
}

