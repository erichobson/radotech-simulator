/**
 * @file DeviceImageWidget.cpp
 * @brief Implementation of the DeviceImageWidget class.
 */

#include "DeviceImageWidget.h"

#include <QResizeEvent>

DeviceImageWidget::DeviceImageWidget(const QPixmap &pixmap, QWidget *parent)
    : QLabel(parent), originalPixmap(pixmap) {
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored);
}

void DeviceImageWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event)
    int availableHeight = height();

    // Scale the pixmap to fit the available height, maintaining aspect ratio
    if (!originalPixmap.isNull()) {
        QPixmap scaledPixmap = originalPixmap.scaledToHeight(
            availableHeight, Qt::SmoothTransformation);
        setPixmap(scaledPixmap);
    }
}
