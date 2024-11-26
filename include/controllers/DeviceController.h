/**
 * @file DeviceController.h
 * @brief Declaration of the DeviceController class.
 */

#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>
#include <QTimer>

class DeviceController : public QObject {
    Q_OBJECT

   public:
    explicit DeviceController(QObject *parent = nullptr);

    /**
     * @brief Checks if the device is currently on.
     * @return True if the device is on, false otherwise.
     */
    bool isDeviceOn() const { return deviceOn; }

    /**
     * @brief Gets the current battery level.
     * @return Battery level percentage (0-100).
     */
    int getBatteryLevel() const { return batteryLevel; }

    /**
     * @brief Checks if the device is currently charging.
     * @return True if charging, false otherwise.
     */
    bool isCharging() const { return charging; }

    /**
     * @brief Checks if the device is connected.
     * @return True if connected, false otherwise.
     */
    bool isConnected() const { return connected; }

    /**
     * @brief Starts connecting the device (simulated connection time).
     *
     * This is just to simulate the pause of the device connecting to the
     * interface, can be removed later if needed.
     */
    void startConnection() { connectionTimer->start(2000); }

    /**
     * @brief Sets the rate at which the device charges.
     * @param rate The rate in int to increase the battery level by.
     */
    void setPowerChargeRate(int rate) { powerChargeRate = rate; }

    /**
     * @brief Sets the rate at which the device consumes power.
     * @param rate The rate in int to decrease the battery level by.
     */
    void setPowerConsumptionRate(int rate) { powerConsumptionRate = rate; }

   public slots:
    /**
     * @brief Sets the device's on/off state.
     * @param isOn True to turn the device on, false to turn it off.
     */
    void setDeviceOn(bool isOn);

    /**
     * @brief Starts charging the device.
     */
    void startCharging();

    /**
     * @brief Stops charging the device.
     */
    void stopCharging();

   signals:
    /**
     * @brief Signal emitted when the device's on/off state changes.
     * @param isOn The new state of the device.
     */
    void deviceStateChanged(bool isOn);

    /**
     * @brief Signal emitted when the battery level changes.
     * @param level The new battery level percentage.
     */
    void batteryLevelChanged(int level);

    /**
     * @brief Signal emitted when the device's connection status changes.
     * @param isConnected The new connection status.
     */
    void connectionStatusChanged(bool isConnected);

    /**
     * @brief Signal emitted when the charging state changes.
     * @param isCharging The new charging state.
     */
    void chargingStateChanged(bool isCharging);

   private slots:
    void updateBatteryLevel();
    void onConnectionTimerTimeout();

   private:
    bool deviceOn;
    int batteryLevel;
    bool charging;
    bool connected;

    QTimer *batteryTimer;
    QTimer *connectionTimer;

    int powerChargeRate;
    int powerConsumptionRate;
};

#endif  // DEVICECONTROLLER_H
