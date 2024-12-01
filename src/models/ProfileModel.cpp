/**
 * @file ProfileModel.cpp
 * @brief A model of the profile.
 */

#include "ProfileModel.h"
#include <QDebug>

ProfileModel::ProfileModel() {}

ProfileModel::ProfileModel(int id, int userId, const QString& name, const QString& desc): 
    id(id), 
    userId(userId),
    name(name), 
    desc(desc) 
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

QString ProfileModel::toString() const {
    return QString("ID: %1\nUser ID: %2\nName: %3\nDescription: %4")
        .arg(id)
        .arg(userId)
        .arg(name)
        .arg(desc);
}

