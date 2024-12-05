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
 * @brief Gets a user by id 
 * @param userId the user id 
 * @param user a reference to the user model object to be populated with the result
 * @return true if the user was found
 */
bool UserController::getUserById(int userId, UserModel& user) const {

    QList<QMap<QString, QVariant>> results;

    try {
        db.query("SELECT * from users WHERE user_id = ?;", {userId}, results);
        
        if (results.isEmpty()) return false;
        const auto& result = results.first();

        user.setId(result.value("user_id").toInt());
        user.setFirstName(result.value("first_name").toString());
        user.setLastName(result.value("last_name").toString());
        user.setEmail(result.value("email").toString());
        user.setPasswordHash(result.value("password_hash").toString());

        return true;

    } catch(const std::exception& e) {
        qCritical() << "Failed to get user: " << e.what();
        return false;
    }
}

/**
 * @brief Gets a user by email
 * @param email the email of the user 
 * @param user a reference to the user model object to be populated with the result
 * @return true if the user was found
 */
bool UserController::getUserByEmail(const QString& email, UserModel& user) const {

    QList<QMap<QString, QVariant>> results;

    try {
        db.query("SELECT * from users WHERE email = ?;", {email}, results);
        
        if (results.isEmpty()) return false;
        const auto& result = results.first();

        user.setId(result.value("user_id").toInt());
        user.setFirstName(result.value("first_name").toString());
        user.setLastName(result.value("last_name").toString());
        user.setEmail(result.value("email").toString());
        user.setPasswordHash(result.value("password_hash").toString());

        return true;

    } catch(const std::exception& e) {
        qCritical() << "Failed to get user: " << e.what();
        return false;
    }
}

/**
 * @brief Gets all profiles associated with a user
 * @param userId the user id 
 * @param profiles reference to a vector that will be populated with the results
 * @return true if the operation was successful
 */
bool UserController::getUserProfiles(int userId, QVector<ProfileModel*>& profiles) const {

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
 * @brief Creates a user
 * @param firstName the first name of the user 
 * @param lastName the last name of the user 
 * @param email the email of the user 
 * @param password the raw password of the user 
 * @param user the user model to populate with the created user 
 * @return true if the operation was successful
 */
bool UserController::createUser(const QString& firstName, const QString& lastName, const QString& email, QString& password, UserModel& user) {
    try {
        QString hashedPass = hash(password);
        db.execute(
            "INSERT INTO users (first_name, last_name, email, password_hash) VALUES (?,?,?,?);", 
            {firstName, lastName, email, hashedPass}
        );

        QList<QMap<QString, QVariant>> results;
        db.query(
            "SELECT * FROM users WHERE email = ? AND password_hash = ?;",
            {email, hashedPass},
            results
        );
        
        if (results.isEmpty()) return false;
        const auto& result = results.first();
        
        user.setId(result.value("profile_id").toInt());
        user.setFirstName(result.value("first_name").toString());
        user.setLastName(result.value("last_name").toString());
        user.setEmail(result.value("email").toString());
        user.setPasswordHash(result.value("password_hash").toString());

        return true;

    } catch(const std::exception& e) {
        qCritical() << "Failed to create user: " << e.what();
        return false;
    }
}

/**
 * @brief Validates a user 
 * @param email the user's email 
 * @param password the user's password 
 * @param user the user model that will get updated with the result if validated
 * @return true if the operation was successful and the user was validated
 */
bool UserController::validateUser(const QString& email, QString& password, UserModel& user) {
    try{
        QString hashedPass = hash(password);

        QList<QMap<QString, QVariant>> results;
        db.query(
            "SELECT * FROM users WHERE email = ? AND password_hash = ?;",
            {email, hashedPass},
            results
        );
        
        if (results.isEmpty()) return false;
        const auto& result = results.first();

        if(result.value("password_hash").toString() != hashedPass || result.value("email").toString() != email) return false;
        
        user.setId(result.value("user_id").toInt());
        user.setFirstName(result.value("first_name").toString());
        user.setLastName(result.value("last_name").toString());
        user.setEmail(result.value("email").toString());
        user.setPasswordHash(result.value("password_hash").toString());

        return true;

    } catch(const std::exception& e) {
            qCritical() << "Failed to create user: " << e.what();
            return false;
    }
}

/**
 * @brief Updates a user 
 * @param id the user id 
 * @param firstName the user's new first name
 * @param lastName the user's new last name 
 * @param email the user's new email 
 * @param password the user's new password 
 * @param user the user model that will get updated with the result 
 * @return true if the operation was successful
 */
bool UserController::updateUser(int id, const QString& firstName, const QString& lastname, const QString& email, QString& password, UserModel& user) {
    try {
        QString hashedPass = hash(password);
        db.execute(
            "UPDATE users SET first_name = ?, last_name = ?, email = ?, password_hash = ? WHERE user_id = ?;", 
            {firstName, lastname, email, hashedPass, id}
        );

        QList<QMap<QString, QVariant>> results;
        db.query(
            "SELECT * FROM users WHERE email = ? AND password_hash = ?;",
            {email, hashedPass},
            results
        );
        
        if (results.isEmpty()) return false;
        const auto& result = results.first();
        
        user.setId(result.value("user_id").toInt());
        user.setFirstName(result.value("first_name").toString());
        user.setLastName(result.value("last_name").toString());
        user.setEmail(result.value("email").toString());
        user.setPasswordHash(result.value("password_hash").toString());

        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to update user: " << e.what();
        return false;
    }
}

/**
 * @brief Updates a user  
 * @param user the user model to update with 
 * @return true if the operation was successful
 */
bool UserController::updateUser(UserModel* user) {
    try {
        QString hashedPass = hash(user->getPasswordHash());
        db.execute(
            "UPDATE users SET first_name = ?, last_name = ?, email = ?, password_hash = ? WHERE user_id = ?;", 
            {user->getFirstName(), user->getLastName(), user->getEmail(), hashedPass, user->getId()}
        );

        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to update user: " << e.what();
        return false;
    }
}

/**
 * @brief Deletes a user  
 * @param userId the user id to delete 
 * @return true if the operation was successful
 */
bool UserController::deleteUser(int userId) {
    try{
        db.execute("DELETE FROM users WHERE user_id = ?;", {userId});
        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to delete user: " << e.what();
        return false;
    }
}

/**
 * @brief Deletes a user 
 * @param user the user model to delete with
 * @return true if the operation was successful
 */
bool UserController::deleteUser(UserModel* user) {
    try{
        db.execute("DELETE FROM users WHERE user_id = ?;", {user->getId()});
        return true;
    } catch(const std::exception& e) {
        qCritical() << "Failed to delete user: " << e.what();
        return false;
    }
}

/**
 * @brief Hashes a string 
 * @param text the hashed text 
 */
QString UserController::hash(const QString& text) {
    QByteArray hashedBytes = QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Sha256);
    QString hashedString = hashedBytes.toHex();
    return hashedString;
}

