/**
 * @file UserProfileController.cpp
 * @brief Controller class that provides CRUD for user profiles.
 */

#include "UserProfileController.h"

/**
 * @brief Constructor for a UserProfileController 
 * @param db a reference to the database manager
 */
UserProfileController::UserProfileController(DatabaseManager& db): db(db) {}

/**
 * @brief Destructor for a UserProfileController 
 */
UserProfileController::~UserProfileController() {}

/**
 * @brief Gets a profile by user id and name
 * @param userId the user id 
 * @param name the name of the profile 
 * @param profile a reference to the profile object to be populated with the result
 * @return true if the profile was found
 */
bool UserProfileController::getProfileByName(int userId, const QString& name, ProfileModel& profile) const {

    QList<QMap<QString, QVariant>> results;

    try {
        db.query("SELECT * from profile WHERE user_id = ? AND name = ?;", {userId, name}, results);
        
        if (results.isEmpty()) return false;
        const auto& result = results.first();

        profile.setId(result.value("profile_id").toInt());
        profile.setUserId(result.value("user_id").toInt());
        profile.setName(result.value("name").toString());
        profile.setDesc(result.value("description").toString());
        profile.setSex(result.value("sex").toString());
        profile.setWeight(result.value("weight").toInt());
        profile.setHeight(result.value("height").toInt());
        profile.setDob(result.value("date_of_birth").toDate());

        return true;

    } catch(const std::exception& e) {
        qCritical() << "Failed to get profile: " << e.what();
        return false;
    }
}

/**
 * @brief Gets all profiles associated with a user
 * @param userId the user id 
 * @param profiles reference to a vector that will be populated with the results
 * @return true if the operation was successful
 */
bool UserProfileController::getProfiles(int userId, QVector<ProfileModel*>& profiles) const {

    QList<QMap<QString, QVariant>> results;

    try {
        db.query("SELECT * from profile WHERE user_id = ?;", {userId}, results);
        profiles.clear();
        for(const auto& result : results) {
            ProfileModel* profile = new ProfileModel(
                result.value("profile_id").toInt(),
                result.value("user_id").toInt(),
                result.value("name").toString(),
                result.value("description").toString(),
                result.value("sex").toString(),
                result.value("weight").toInt(),
                result.value("height").toInt(),
                result.value("date_of_birth").toDate()
            );
            profiles.append(profile);
        }
        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to get profiles: " << e.what();
        return false;
    }
}

/**
 * @brief Creates a profile 
 * @param userId the user id associated with the profile 
 * @param name the name of the profile 
 * @param desc the description of the profile 
 * @return true if the operation was successful
 */
bool UserProfileController::createProfile(int userId, const QString& name, const QString& desc, const QString& sex, int weight, int height, const QDate& dob) {
    try {
        QString dobString = dob.toString("yyyy-MM-dd");
        db.execute(
            "INSERT INTO profile (user_id, name, description, sex, weight, height, date_of_birth) VALUES (?,?,?,?,?,?,?);", 
            {userId, name, desc, sex, weight, height, dobString}
        );
        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to create profile: " << e.what();
        return false;
    }
}

/**
 * @brief Creates a profile 
 * @param profile the profile model to create the profile with 
 * @return true if the operation was successful
 */
bool UserProfileController::createProfile(ProfileModel* profile) {
    try {
        db.execute(
            "INSERT INTO profile (user_id, name, description, sex, weight, height, date_of_birth) VALUES (?,?,?,?,?,?,?);", 
            {profile->getUserId(), profile->getName(), profile->getDesc(), profile->getSex(), profile->getWeight(), profile->getHeight(), profile->getDobString()}
        );
        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to createe profile: " << e.what();
        return false;
    }
}

/**
 * @brief Updates a profile 
 * @param profileId the profile id 
 * @param userId the user id associated with the profile
 * @param name the name to update the profile with 
 * @param desc the description to update the profile with 
 * @return true if the operation was successful
 */
bool UserProfileController::updateProfile(int profileId, int userId, const QString& name, const QString& desc, const QString& sex, int weight, int height, const QDate& dob) {
    try {
        QString dobString = dob.toString("yyyy-MM-dd");
        db.execute(
            "UPDATE profile SET user_id = ?, name = ?, description = ?, sex = ?, weight = ?, height = ?, date_of_birth = ? WHERE profile_id = ?;", 
            {userId, name, desc, sex, weight, height, dobString, profileId}
        );

        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to update profile: " << e.what();
        return false;
    }
}

/**
 * @brief Updates a profile 
 * @param profile the profile model to update the profile with 
 * @return true if the operation was successful
 */
bool UserProfileController::updateProfile(ProfileModel* profile) {
    try {
        db.execute(
            "UPDATE profile SET user_id = ?, name = ?, description = ?, sex = ?, weight = ?, height = ?, date_of_birth = ? WHERE profile_id = ?;", 
            {profile->getUserId(), profile->getName(), profile->getDesc(), profile->getSex(), profile->getWeight(), profile->getHeight(), profile->getDobString(), profile->getId()}
        );

        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to update profile: " << e.what();
        return false;
    }
}

/**
 * @brief Deletes a profile 
 * @param profileId the profile id to delete 
 * @return true if the operation was successful
 */
bool UserProfileController::deleteProfile(int profileId) {
    try{
        db.execute("DELETE FROM profile WHERE profile_id = ?;", {profileId});
        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to delete profile: " << e.what();
        return false;
    }
}

/**
 * @brief Deletes a profile 
 * @param profile the profile model to delete with
 * @return true if the operation was successful
 */
bool UserProfileController::deleteProfile(ProfileModel* profile) {
    try{
        db.execute("DELETE FROM profile WHERE profile_id = ?;", {profile->getId()});
        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to delete profile: " << e.what();
        return false;
    }
}

/**
 * @brief Gets all the scans associated with a profile 
 * @param profileId the profile id 
 * @param scans a reference to a vector that will be populated with the results
 * @return true if the operation was successful
 */
bool UserProfileController::getProfileScans(int profileId, QVector<ScanModel*>& scans) const {

    QList<QMap<QString, QVariant>> results;
    try{
        db.query("SELECT * FROM scan WHERE profile_id = ?;", {profileId}, results);

        scans.clear();
        for(const auto& result : results) {
            ScanModel* scan = new ScanModel(
                result.value("scan_id").toInt(),
                result.value("profile_id").toInt(),
                result.value("h1_lung").toInt(),
                result.value("h1_lung_r").toInt(),
                result.value("h2_heart_constrictor").toInt(),
                result.value("h2_heart_constrictor_r").toInt(),
                result.value("h3_heart").toInt(),
                result.value("h3_heart_r").toInt(),
                result.value("h4_small_intestine").toInt(),
                result.value("h4_small_intestine_r").toInt(),
                result.value("h5_triple_heater").toInt(),
                result.value("h5_triple_heater_r").toInt(),
                result.value("h6_large_intestine").toInt(),
                result.value("h6_large_intestine_r").toInt(),

                result.value("f1_spleen").toInt(),
                result.value("f1_spleen_r").toInt(),
                result.value("f2_liver").toInt(),
                result.value("f2_liver_r").toInt(),
                result.value("f3_kidney").toInt(),
                result.value("f3_kidney_r").toInt(),
                result.value("f4_urinary_bladder").toInt(),
                result.value("f4_urinary_bladder_r").toInt(),
                result.value("f5_gull_bladder").toInt(),
                result.value("f5_gull_bladder_r").toInt(),
                result.value("f6_stomach").toInt(),
                result.value("f6_stomach_r").toInt(),

                result.value("created_on").toDate(),

                result.value("body_temp").toInt(),
                result.value("blood_pressure").toInt(),
                result.value("heart_rate").toInt(),
                result.value("sleeping_time").toInt(),
                result.value("current_weight").toInt(),
                result.value("emotional_state").toInt(),
                result.value("overall_feeling").toInt(),
                result.value("name").toString(),
                result.value("notes").toString()
            );

            scans.append(scan);
        } 

        return true;

    } catch(const std::exception& e) {
        qCritical() << "Failed to get profile scans: " << e.what();
        return false;
    }
}

