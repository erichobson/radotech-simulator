#include "SettingsWidget.h"

#include <QLabel>
#include <QVBoxLayout>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent) {
    QLabel *label = new QLabel("Settings Placeholder");
    label->setStyleSheet("font-size: 20px; color: #333333;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
}
