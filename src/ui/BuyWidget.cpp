#include "BuyWidget.h"

#include <QLabel>
#include <QVBoxLayout>

BuyWidget::BuyWidget(QWidget *parent) : QWidget(parent) {
    QLabel *label = new QLabel("Buy Placeholder");
    label->setStyleSheet("font-size: 20px; color: #333333;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
}
