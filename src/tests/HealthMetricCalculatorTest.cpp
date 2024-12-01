/**
 * @file HealthMetricCalculatorTest.cpp
 * @brief Tests for the HealthMetricCalculator class.
 */

#include "HealthMetricCalculatorTest.h"

HealthMetricCalculatorTest::HealthMetricCalculatorTest() {}
HealthMetricCalculatorTest::~HealthMetricCalculatorTest() {}

bool HealthMetricCalculatorTest::test() const {

    HealthMetricCalculator* hmc = new HealthMetricCalculator();
    QVector<HealthMetricModel*> hms;
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
    int f3 = 75;
    int f4 = 58;
    int f5 = 25;
    int f6 = 27;
    
    int f1r = 75;
    int f2r = 52;
    int f3r = 57;
    int f4r = 58;
    int f5r = 25;
    int f6r = 20;
    
    QDate createdOn(2000,6,6);

    int bt = 150;
    int bp = 160;
    int hr = 188;
    int st = 2232;
    int cw = 185;
    int em = 1;
    int ofeel = 2;
    QString name = "Andrew scan";
    QString notes = "Test note";

    ScanModel newScan(
        id,profileId,
        h1,h1r,h2,h2r,h3,h3r,h4,h4r,h5,h5r,h6,h6r,
        f1,f1r,f2,f2r,f3,f3r,f4,f4r,f5,f5r,f6,f6r,
        createdOn,
        bt,bp,hr,st,cw,em,ofeel,name,notes
    );

    ScanModel newScan2(
        (id + 1),profileId,
        h1 + 1,h1r-1,h2-2,h2r+2,h3+3,h3r+2,h4-4,h4r+5,h5+6,h5r-7,h6-2,h6r+1,
        f1 + 1,f1r-2,f2-3,f2r+3,f3+4,f3r+5,f4+3,f4r-2,f5+1,f5r+4,f6-2,f6r+3,
        createdOn,
        bt,bp,hr,st,cw,em,ofeel,name,notes
    );

    const QVector<ScanModel*> scans = {&newScan, &newScan2};

    qDebug() << "Calculating Organ Health";
    if(!(hmc->calculateOrganHealth(&newScan, hms))) return false;
    for(auto* hm : hms){
        qDebug() << hm->toString();
        delete hm;
    }

    qDebug() << "Calculating Indicator Health";
    if(!(hmc->calculateIndicatorHealth(&newScan, hms))) return false;
    for(auto* hm : hms){
        qDebug() << hm->toString();
        delete hm;
    }

    qDebug() << "Calculating Trend Indicators";
    if(!(hmc->calculateTrendHealth(scans, hms))) return false;
    for(auto* hm : hms){
        qDebug() << hm->toString();
        delete hm;
    }
    delete hmc;
    return true;
}

