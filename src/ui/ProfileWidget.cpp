/**
 * @file ProfileWidget.cpp
 * @brief Implementation of the ProfileWidget class.
 */

#include "ProfileWidget.h"

#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QVBoxLayout>

ProfileWidget::ProfileWidget(const QString &imagePath, const QString &userName,
                             QWidget *parent)
    : QWidget(parent) {
    profilePicLabel = new QLabel;
    userNameLabel = new QLabel(userName);

    // Create the circular masked pixmap
    QPixmap originalPixmap(imagePath);
    if (!originalPixmap.isNull()) {
        int size = 100;

        // Scale and mask the pixmap
        QPixmap scaledPixmap = originalPixmap.scaled(
            size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QPixmap circularPixmap(size, size);
        circularPixmap.fill(Qt::transparent);

        QPainterPath path;
        path.addEllipse(0, 0, size, size);

        QPainter painter(&circularPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, scaledPixmap);
        painter.end();

        // Set the circular pixmap as the QLabel pixmap
        profilePicLabel->setPixmap(circularPixmap);
        profilePicLabel->setAlignment(Qt::AlignCenter);
    } else {
        // Placeholder circle if image not available
        profilePicLabel->setFixedSize(100, 100);
        profilePicLabel->setStyleSheet(
            "background-color: #CCCCCC; border-radius: 50px; width: 100px; "
            "height: 100px;");
        profilePicLabel->setAlignment(Qt::AlignCenter);
    }

    // Style the username label
    userNameLabel->setAlignment(Qt::AlignCenter);
    userNameLabel->setStyleSheet(
        "color: white; font-size: 14pt; font-weight: bold;");

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(profilePicLabel);
    layout->addWidget(userNameLabel);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 20, 0, 20);
}
