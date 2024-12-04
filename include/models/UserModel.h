/**
 * @file UserModel.h
 * @brief A model of the user.
 */

#ifndef USER_MODEL_H
#define USER_MODEL_H

#include <QString>
class UserModel {
   public:
    UserModel();
    UserModel(int, const QString&, const QString&, const QString&,
              const QString&);
    ~UserModel();

    int getId() const;
    void setId(int);

    QString getFirstName() const;
    void setFirstName(const QString&);

    QString getLastName() const;
    void setLastName(const QString&);

    QString getEmail() const;
    void setEmail(const QString&);

    QString getPasswordHash() const;
    void setPasswordHash(const QString&);

    QString toString() const;

   private:
    int id;
    QString firstName;
    QString lastName;
    QString email;
    QString passwordHash;
};

#endif
