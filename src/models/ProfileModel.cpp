/**
 * @file ProfileModel.cpp
 * @brief A model of the profile.
 */

#include "ProfileModel.h"
#include <QDebug>

ProfileModel::ProfileModel() {}

ProfileModel::ProfileModel(int id, int userId, const QString& name, const QString& desc, const QString& sex, int weight, int height, const QDate& dob): 
    id(id), 
    userId(userId),
    name(name), 
    desc(desc),
    sex(sex),
    weight(weight),
    height(height),
    dob(dob)
{}

ProfileModel::~ProfileModel() {}

int ProfileModel::getId() const {
    return id;
}

void ProfileModel::setId(int id) {
    this->id = id;
}

int ProfileModel::getUserId() const {
    return userId;
}

void ProfileModel::setUserId(int id) {
    this->userId = id;
}

QString ProfileModel::getName() const {
    return name;
}

void ProfileModel::setName(const QString& name) {
    this->name = name;
}

QString ProfileModel::getDesc() const {
    return desc;
}

void ProfileModel::setDesc(const QString& desc) {
    this->desc = desc;
}

QString ProfileModel::getSex() const {
    return sex;
}

void ProfileModel::setSex(const QString& sex) {
    this->sex = sex;
}

int ProfileModel::getWeight() const {
    return weight;
}

void ProfileModel::setWeight(int weight) {
    this->weight = weight;
}

int ProfileModel::getHeight() const {
    return height;
}

void ProfileModel::setHeight(int height) {
    this->height = height;
}
        
QDate ProfileModel::getDob() const {
    return dob;
}

QString ProfileModel::getDobString() const {
    return dob.toString("yyyy-MM-dd");
}

void ProfileModel::setDob(const QDate& dob) {
    this->dob = dob;
}

QString ProfileModel::toString() const {
    return QString("ID: %1\nUser ID: %2\nName: %3\nDescription: %4\nSex: %5\nWeight: %6\nHeight: %7\nDob: %8")
        .arg(id)
        .arg(userId)
        .arg(name)
        .arg(desc)
        .arg(sex)
        .arg(weight)
        .arg(height)
        .arg(this->getDobString());
}

