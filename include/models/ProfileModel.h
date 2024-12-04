/**
 * @file ProfileModel.h
 * @brief A model of the profile.
 */

#ifndef PROFILE_MODEL_H
#define PROFILE_MODEL_H

#include <QDate>
#include <QString>

class ProfileModel {
   public:
    ProfileModel();
    ProfileModel(int, int, const QString&, const QString&, int, int,
                 const QDate&);

    ~ProfileModel();

    int getId() const;
    void setId(int);

    int getUserId() const;
    void setUserId(int);

    QString getName() const;
    void setName(const QString&);

    QString getSex() const;
    void setSex(const QString&);

    int getWeight() const;
    void setWeight(int);

    int getHeight() const;
    void setHeight(int);

    QDate getDob() const;
    void setDob(const QDate&);

    QString toString() const;

   private:
    int id;
    int userId;
    QString name;
    QString sex;
    int weight;  // kg
    int height;  // cm
    QDate dob;
};

#endif
