#include "HomeWidget.h"

#include <QLabel>
#include <QVBoxLayout>

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent) {
    QLabel *label = new QLabel("Home Placeholder");
    label->setStyleSheet("font-size: 20px; color: #333333;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
}
