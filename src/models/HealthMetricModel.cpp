/**
 * @file src/models/HealthMetricModel.cpp
 * @brief A model for the health metrics
 */

#include "HealthMetricModel.h"

HealthMetricModel::HealthMetricModel() {}
HealthMetricModel::HealthMetricModel(QString name, float value, QString desc, int level): name(name), value(value), desc(desc), level(level) {}
HealthMetricModel::~HealthMetricModel() {}

QString HealthMetricModel::getName() const { return name; }
void HealthMetricModel::setName(QString name) { this->name = name; }

float HealthMetricModel::getValue() const { return value; }
void HealthMetricModel::setValue(float value) { this->value = value; }

QString HealthMetricModel::getDesc() const { return desc; }
void HealthMetricModel::setDesc(QString desc) { this->desc = desc; }

int HealthMetricModel::getLevel() const { return level; }
void HealthMetricModel::setLevel(int level) { this->level = level; }

QString HealthMetricModel::toString() {
    return QString("Name: %1\nValue: %2\nDesc: %3\nLevel:%4")
        .arg(name)
        .arg(value)
        .arg(desc)
        .arg(level);
}

