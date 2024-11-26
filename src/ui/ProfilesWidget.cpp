#include "ProfilesWidget.h"

#include <QLabel>
#include <QVBoxLayout>

ProfilesWidget::ProfilesWidget(QWidget *parent) : QWidget(parent) {
    QLabel *label = new QLabel("Profiles Placeholder");
    label->setStyleSheet("font-size: 20px; color: #333333;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
}
