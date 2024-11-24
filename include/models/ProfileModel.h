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
        ProfileModel(int, int, const QString&, const QString& = "");
        ~ProfileModel();

        int getId() const;
        void setId(int);

        int getUserId() const;
        void setUserId(int);

        QString getName() const;
        void setName(const QString&);

        QString getDesc() const;
        void setDesc(const QString&);

        QString toString() const;
        void test();

    private:
        int id;
        int userId;
        QString name;
        QString desc;
};

#endif
