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
    const QString& sex, 
    int weight, 
    int height, 
    const QDate& dob, 
    const QString& email, 
    const QString& passwordHash
): 
    id(id), 
    firstName(fName), 
    lastName(lName), 
    sex(sex), 
    weight(weight), 
    height(height), 
    dob(dob), 
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

QString UserModel::getSex() const {
    return sex;
}

void UserModel::setSex(const QString& sex) {
    this->sex = sex;
}

int UserModel::getWeight() const {
    return weight;
}

void UserModel::setWeight(int weight) {
    this->weight = weight;
}

int UserModel::getHeight() const {
    return height;
}

void UserModel::setHeight(int height) {
    this->height = height;
}

QDate UserModel::getDob() const {
    return dob;
}

void UserModel::setDob(const QDate& dob) {
    this->dob = dob;
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
    return QString("ID: %1\nFirst Name: %2\nLast Name: %3\nSex: %4\nWeight: %5\nHeight: %6\nDOB: %7\nEmail: %8")
        .arg(id)
        .arg(firstName)
        .arg(lastName)
        .arg(sex)
        .arg(weight)
        .arg(height)
        .arg(dob.toString("yyyy-MM-dd"))  // Format the date to a readable format
        .arg(email);
}

