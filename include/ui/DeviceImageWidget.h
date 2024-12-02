/**
 * @file DeviceImageWidget.h
 * @brief Declaration of the DeviceImageWidget class.
 *
 * This widget displays the device image, scales it to fit the available height
 * while maintaining the aspect ratio, and shows a green indicator when the
 * device is powered on.
 */

#ifndef DEVICEIMAGEWIDGET_H
#define DEVICEIMAGEWIDGET_H

#include <QLabel>
#include <QPixmap>

class DeviceController;

class DeviceImageWidget : public QLabel {
    Q_OBJECT

   public:
    explicit DeviceImageWidget(const QPixmap &pixmap,
                               QWidget *parent = nullptr);

    /**
     * @brief Sets the DeviceController instance for managing device state.
     * @param controller Pointer to the DeviceController instance.
     */
    void setDeviceController(DeviceController *controller);

   signals:
    void imageTouchingEdge();
    void imageReleased();

   protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

   private slots:
    /**
     * @brief Slot called when the device state changes.
     * @param isOn True if the device is powered on, false otherwise.
     */
    void onDeviceStateChanged(bool isOn);

   private:
    QPoint originalPosition;
    bool isMoving;
    int maxMovement;  // Maximum pixels to move down
    bool powerButtonOn;
    QRect powerButtonRect;

    QPixmap originalPixmap;
    QPixmap scaledPixmap;

    DeviceController *deviceController;
};

#endif  // DEVICEIMAGEWIDGET_H
