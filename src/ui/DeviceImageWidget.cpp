/**
 * @file DeviceImageWidget.cpp
 * @brief Implementation of the DeviceImageWidget class.
 */

#include "DeviceImageWidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QResizeEvent>

#include "DeviceController.h"

/* Logging Macros */
#define DEBUG(msg) qDebug() << "[DEBUG]" << __FUNCTION__ << ":" << msg
#define INFO(msg) qInfo() << "[INFO]" << __FUNCTION__ << ":" << msg
#define ERROR(msg) qCritical() << "[ERROR]" << __FUNCTION__ << ":" << msg

DeviceImageWidget::DeviceImageWidget(const QPixmap &pixmap, QWidget *parent)
    : QLabel(parent),
      isMoving(false),
      maxMovement(50),
      powerButtonOn(false),
      powerButtonRect(),
      originalPixmap(pixmap),
      scaledPixmap(),
      deviceController(nullptr) {
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
}

void DeviceImageWidget::mousePressEvent(QMouseEvent *event) {
    if (powerButtonRect.contains(event->pos())) {
        if (deviceController) {
            bool newState = !deviceController->isDeviceOn();
            deviceController->setDeviceOn(newState);
        } else {
            ERROR("DeviceController not set");
            powerButtonOn = !powerButtonOn;
            update();
        }
    } else {
        if (!isMoving) {
            originalPosition = pos();
            isMoving = true;

            QWidget *parentWidget = qobject_cast<QWidget *>(parent());
            if (parentWidget) {
                maxMovement = parentWidget->height() - (pos().y() + height());
                maxMovement = qMin(maxMovement, 50);
            }

            QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
            animation->setDuration(150);
            animation->setStartValue(pos());
            animation->setEndValue(QPoint(pos().x(), pos().y() + maxMovement));
            animation->setEasingCurve(QEasingCurve::OutCubic);

            // Connect to animation finished to emit the signal
            connect(animation, &QPropertyAnimation::finished, this,
                    [this]() { emit imageTouchingEdge(); });

            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}

void DeviceImageWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (isMoving) {
        emit imageReleased();  // Emit signal when released

        QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
        animation->setDuration(150);
        animation->setStartValue(pos());
        animation->setEndValue(originalPosition);
        animation->setEasingCurve(QEasingCurve::InCubic);

        connect(animation, &QPropertyAnimation::finished, this,
                [this]() { isMoving = false; });

        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QLabel::mouseReleaseEvent(event);
}

void DeviceImageWidget::setDeviceController(DeviceController *controller) {
    deviceController = controller;
    if (deviceController) {
        // Connect to the deviceStateChanged signal to update the widget
        connect(deviceController, &DeviceController::deviceStateChanged, this,
                &DeviceImageWidget::onDeviceStateChanged);

        // Initialize the powerButtonOn state from the DeviceController
        powerButtonOn = deviceController->isDeviceOn();
        update();
    } else {
        ERROR("DeviceController not set");
    }
}

void DeviceImageWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    // Verify that the original pixmap is not null
    if (!originalPixmap.isNull()) {
        scaledPixmap =
            originalPixmap.scaledToHeight(height(), Qt::SmoothTransformation);

        // Set the scaled pixmap
        setPixmap(scaledPixmap);

        // Location of power button on image
        double xRatio = 0.47;
        double yRatio = 0.08;
        double buttonSizeRatio = 0.43;

        // Determine the size of the button
        double buttonSize = scaledPixmap.width() * buttonSizeRatio;

        // Determine the location of the image
        int imageX = (width() - scaledPixmap.width()) / 2;
        int imageY = (height() - scaledPixmap.height()) / 2;

        // Determine the placement for the button
        int buttonX = imageX + scaledPixmap.width() * xRatio - buttonSize / 2;
        int buttonY = imageY + scaledPixmap.height() * yRatio - buttonSize / 2;

        // Set the power button rectangle
        powerButtonRect = QRect(buttonX, buttonY, buttonSize, buttonSize);
    } else {
        ERROR("Original pixmap is null");
    }
}

void DeviceImageWidget::paintEvent(QPaintEvent *event) {
    QLabel::paintEvent(event);

    // Draw a green overlay if device is on
    if (powerButtonOn) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setBrush(QColor(0, 255, 0, 40));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(powerButtonRect);
    }
}

void DeviceImageWidget::onDeviceStateChanged(bool isOn) {
    powerButtonOn = isOn;
    update();
}
