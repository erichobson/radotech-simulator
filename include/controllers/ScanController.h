/**
 * @file ScanController.h
 * @brief Declaration of the ScanController class.
 */
#ifndef SCANCONTROLLER_H
#define SCANCONTROLLER_H

#include <QString>
#include <QDate>
#include <QVector>

#include "DatabaseManager.h"
#include "ScanModel.h"
#include "ProfileModel.h"

#define SCAN_POINTS         (24)
class ScanController {
    public:
        ScanController(DatabaseManager&);
        void createScan(const QVector<int>&, ProfileModel&);
        int generateMeasurement(int, int);
        bool storeScan(ScanModel&);

    private:
        DatabaseManager& db;
};
#endif // SCANCONTROLLER_H
