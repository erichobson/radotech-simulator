/**
 * @file include/models/ProfileModel.h
 * @brief A model of the profile
 */

#ifndef PROFILE_MODEL_H
#define PROFILE_MODEL_H

#include <QString>
class ProfileModel {

    public:
        ProfileModel();
        ProfileModel(int, int, const QString&, const QString&);
        ~ProfileModel();

        int getId() const;
        int getUserId() const;
        QString getName() const;
        QString getDesc() const;

        void setId(int);
        void setUserId(int);
        void setName(const QString&);
        void setDesc(const QString&);

        QString toString() const;

    private:
        int id;
        int userId;
        QString name;
        QString desc;
};

#endif
