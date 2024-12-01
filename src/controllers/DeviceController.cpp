/**
 * @file DeviceController.cpp
 * @brief Implementation of the DeviceController class.
 */

#include "DeviceController.h"

#include <QDebug>
#include <QRandomGenerator>

/* Logging Macros */
#define DEBUG(msg) qDebug() << "[DEBUG]" << __FUNCTION__ << ":" << msg
#define INFO(msg) qInfo() << "[INFO]" << __FUNCTION__ << ":" << msg
#define ERROR(msg) qCritical() << "[ERROR]" << __FUNCTION__ << ":" << msg

DeviceController::DeviceController(QObject *parent)
    : QObject(parent),
      deviceOn(false),
      batteryLevel(100),
      charging(false),
      connected(false),
      powerChargeRate(1),
      powerConsumptionRate(1) {
    // Initialize the battery timer
    batteryTimer = new QTimer(this);
    connect(batteryTimer, &QTimer::timeout, this,
            &DeviceController::updateBatteryLevel);
    batteryTimer->start(1000);

    // Initialize the connection timer
    connectionTimer = new QTimer(this);
    connectionTimer->setSingleShot(true);
    connect(connectionTimer, &QTimer::timeout, this,
            &DeviceController::onConnectionTimerTimeout);
}

void DeviceController::setDeviceOn(bool isOn) {
    // Check if battery is depleted
    if (isOn && batteryLevel <= 0) {
        DEBUG("Cannot turn on device. Battery is depleted.");
        return;
    }

    // Otherwise continue with operation
    if (deviceOn != isOn) {
        deviceOn = isOn;
        emit deviceStateChanged(deviceOn);
        if (deviceOn) {
            DEBUG("Starting connection");
            startConnection();
        } else {
            DEBUG("Stopping connection");
            connected = false;
            emit connectionStatusChanged(connected);
        }
    }
}

void DeviceController::startCharging() {
    if (!charging) {
        INFO("Charging started");
        charging = true;
        emit chargingStateChanged(charging);
    } else {
        ERROR("Charging already started");
    }
}

void DeviceController::stopCharging() {
    if (charging) {
        INFO("Charging stopped");
        charging = false;
        emit chargingStateChanged(charging);
    } else {
        ERROR("Charging already stopped");
    }
}

void DeviceController::onConnectionTimerTimeout() {
    connected = true;
    DEBUG("Connected");
    emit connectionStatusChanged(connected);
}

void DeviceController::updateBatteryLevel() {
    if (charging) {
        if (batteryLevel < 100) {
            batteryLevel += powerChargeRate;

            // Stop if battery is fully charged
            if (batteryLevel >= 100) {
                INFO("Battery fully charged");
                batteryLevel = 100;
                stopCharging();
            }
        }

    } else if (deviceOn) {
        if (batteryLevel > 0) {
            batteryLevel -= powerConsumptionRate;

            // Stop when battery is depleted
            if (batteryLevel <= 0) {
                INFO("Battery depleted");
                setDeviceOn(false);
                batteryLevel = 0;
            }
        }
    } else {
        DEBUG("Device is off");
    }
    emit batteryLevelChanged(batteryLevel);
}

void DeviceController::transmitData() {
    int randomData = QRandomGenerator::global()->bounded(
        0, 100);  // Random data between 0 and 99
    emit dataReceived(randomData);
}
