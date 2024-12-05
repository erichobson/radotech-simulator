/**
 * @file UserController.cpp
 * @brief Controller class that provides CRUD for users.
 */

#include "UserController.h"

/**
 * @brief Constructor for a UserController 
 * @param db a reference to the database manager
 */
UserController::UserController(DatabaseManager& db): db(db) {}

/**
 * @brief Destructor for a UserController 
 */
UserController::~UserController() {}


/**
 * @brief Creates a User 
 * @return true if the operation was successful
 */
bool UserController::createUser(const QString &firstName, const QString &lastName,const QString &sex, int weight, int height, const QDate &dob, const QString &email, const QString &password) {
    try {
        db.execute(
            "INSERT INTO users (first_name, last_name, email, password_hash) VALUES (?,?,?,?);", 
            {firstName, lastName, email, password}
        );
        const QString fullName = firstName + " " + lastName;
        const QString description = fullName + "'s Profile";
        int userId = this->getUserId(email);
        emit createProfile(userId, fullName, description, sex, weight, height, dob)
        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to create user: " << e.what();
        return false;
    }
}




/**
 * @brief Creates a User
 * @return true if the operation was successful
 */
bool UserController::createUser(UserModel* user) {
    try {
        db.execute(
            "INSERT INTO users (first_name, last_name, email, password_hash) VALUES (?,?,?,?);", 
            {user->getFirstName(), user->getLastName(), user->getEmail(), user->getPasswordHash()}
        );
        const QString fullName = firstName + " " + lastName;
        const QString description = fullName + "'s Profile";
        int userId = this->getUserId(email);
        emit createProfile(userId, fullName, description, sex, weight, height, dob)
        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to create user: " << e.what();
        return false;
    }
}
/**
 * @brief Gets a User by ID
 * @return true if the operation was successful
 */

bool UserController::getUserById(int userId, UserModel& user) const {
    try {
        QString queryString = "SELECT * from users WHERE user_id = ?;";
        QList<QVariant> params = { userId };
        QList<QMap<QString, QVariant>> results;

        db.query(queryString, params, results);
        if (!results.isEmpty()) {
            const auto& result = results.first();
            user.setId(result.value("user_id").toInt());
            user.setFirstName(result.value("first_name").toString());
            user.setLastName(result.value("last_name").toString());
            user.setEmail(result.value("email").toString());
            user.setPasswordHash(result.value("password_hash").toString());
            return true;
        } else {
            qDebug() << "No user found with given id:" << userId;
            return false; 
        }
    } catch(const std::exception& e) {
        qCritical() << "Failed to get user: " << e.what();
        return false;
    }
}

/**
 * @brief Updates a User  
 * @return true if the operation was successful
 */
bool UserController::updateUser(int userId, const QString &firstName, const QString &lastName,const QString &email, const QString &password) {
    try {
        QString dobString = dob.toString("yyyy-MM-dd");
        int userId = this->getUserId(email);
        db.execute(
            "UPDATE users SET first_name = ?, last_name = ?,  email = ?, password_hash = ? WHERE user_id = ?;", 
            {firstName, lastName, email, password, userId}
        );

        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to update user: " << e.what();
        return false;
    }
}

/**
 * @brief Updates a User 
 * @return true if the operation was successful
 */
bool UserController::updateUser(UserModel* user) {
    try {
        db.execute(
            "UPDATE users SET first_name = ?, last_name = ?,  email = ?, password_hash = ? WHERE user_id = ?;", 
            {user->getFirstName(), user->getLastName(), user->getEmail(), user->getPasswordHash(), user->getId()}
        );

        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to update user: " << e.what();
        return false;
    }
}

/**
 * @brief Gets all the Profiles assigned to a User
 * @return true if the operation was successful
 */

bool UserController::getUserProfiles(int userId, QVector<ProfileModel*>& profiles) const {
    try {

        QString queryString = "SELECT * from profile WHERE user_id = ?;";
        QList<QVariant> params = { userId };
        QList<QMap<QString, QVariant>> results;
        db.query(queryString, params, results);
        profiles.clear();
        if (!results.isEmpty()) {
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
        } else {
            qDebug() << "User has no profiles assigned";
            return false;
        }
        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to get profiles: " << e.what();
        return false;
    }
}

/**
 * @brief Gets a users ID by email
 * @return The Users ID
 */
int UserController::getUserId(const QString &email){
    try {
        QString queryString = "SELECT user_id FROM users WHERE email = ?;";
        QList<QVariant> params = { email };
        QList<QMap<QString, QVariant>> results;

        db.query(queryString, params, results);

        if (!results.isEmpty()) {
            return results.first()["user_id"].toInt(); // Convert the ID to an integer
        } else {
            qDebug() << "No user found with email:" << email;
            return -1; // Return -1 if no user is found
        }
    } catch (const std::exception &e) {
        qCritical() << "Exception occurred while fetching user ID:" << e.what();
        return -1; 
    }
}