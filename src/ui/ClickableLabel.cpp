/**
 * @file ClickableLabel.cpp
 * @brief Implementation of the ClickableLabel class.
 */

#include "ClickableLabel.h"

#include <QMouseEvent>

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent) {
    setCursor(Qt::PointingHandCursor);
}

void ClickableLabel::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    emit clicked();
}
