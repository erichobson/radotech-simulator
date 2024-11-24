/**
 * @file include/models/UserModel.h
 * @brief A model of the user.
 */

#ifndef USER_MODEL_H
#define USER_MODEL_H

#include <QString>
#include <QDate>
class UserModel {

    public:
        UserModel(int, const QString&, const QString&, const QString&, int, int, const QDate&, const QString&, const QString&);
        ~UserModel();

        int getId() const;
        void setId(int);

        QString getFirstName() const;
        void setFirstName(const QString&);

        QString getLastName() const;
        void setLastName(const QString&);

        QString getSex() const;
        void setSex(const QString&);

        int getWeight() const;
        void setWeight(int);

        int getHeight() const;
        void setHeight(int);

        QDate getDob() const;
        void setDob(const QDate&);

        QString getEmail() const;
        void setEmail(const QString&);
        
        QString getPasswordHash() const;
        void setPasswordHash(const QString&);

        QString toString() const;

    private:
        int id;
        QString firstName;
        QString lastName;
        QString sex;
        int weight; // kg
        int height; // cm
        QDate dob;
        QString email;
        QString passwordHash;
};

#endif
