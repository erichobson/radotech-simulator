/**
 * @file UserModel.cpp
 * @brief A model of the user.
 */

#include "UserModel.h"
#include <QDebug>

UserModel::UserModel() {}

UserModel::UserModel(
    int id, 
    const QString& fName, 
    const QString& lName, 
    const QString& email, 
    const QString& passwordHash
): 
    id(id), 
    firstName(fName), 
    lastName(lName), 
    email(email),
    passwordHash(passwordHash)
{}

UserModel::~UserModel() {}

int UserModel::getId() const {
    return id;
}

void UserModel::setId(int id) {
    this->id = id;
}

QString UserModel::getFirstName() const {
    return firstName;
}

void UserModel::setFirstName(const QString& fName) {
    this->firstName = fName;
}

QString UserModel::getLastName() const {
    return lastName;
}

void UserModel::setLastName(const QString& lName) {
    this->lastName = lName;
}

QString UserModel::getEmail() const {
    return email;
}

void UserModel::setEmail(const QString& email) {
    this->email = email;
}

QString UserModel::getPasswordHash() const {
    return passwordHash;
}

void UserModel::setPasswordHash(const QString& pass) {
    this->passwordHash = pass;
}

QString UserModel::toString() const {
    return QString("ID: %1\nFirst Name: %2\nLast Name: %3\nEmail: %4")
        .arg(id)
        .arg(firstName)
        .arg(lastName)
        .arg(email);
}

