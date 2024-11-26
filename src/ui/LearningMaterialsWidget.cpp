#include "LearningMaterialsWidget.h"

#include <QLabel>
#include <QVBoxLayout>

LearningMaterialsWidget::LearningMaterialsWidget(QWidget *parent)
    : QWidget(parent) {
    QLabel *label = new QLabel("Learning Materials Placeholder");
    label->setStyleSheet("font-size: 20px; color: #333333;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
}
