#include "MeasureNowWidget.h"

#include <QLabel>
#include <QVBoxLayout>

MeasureNowWidget::MeasureNowWidget(QWidget *parent) : QWidget(parent) {
    QLabel *label = new QLabel("Measure Now Placeholder");
    label->setStyleSheet("font-size: 20px; color: #333333;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
}
