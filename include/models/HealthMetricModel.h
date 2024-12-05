/**
 * @file HealthMetricModel.h
 * @brief A model for the health metrics.
 */

#ifndef HEALTH_METRIC_MODEL_H
#define HEALTH_METRIC_MODEL_H

#include <QString>
class HealthMetricModel {
   public:
    HealthMetricModel();
    HealthMetricModel(QString, float, QString, int);
    ~HealthMetricModel();

    QString getName() const;
    void setName(QString);

    float getValue() const;
    void setValue(float);

    QString getDesc() const;
    void setDesc(QString);

    int getLevel() const;
    void setLevel(int);

    QString toString();

   private:
    QString name;
    float value;
    QString desc;
    int level;  // -1 below normal, 0 if normal, +1 if above normal
};

#endif
