/**
 * @file ProfilesWidget.h
 * @brief Declaration of the ProfilesWidget class.
 */

#ifndef PROFILESWIDGET_H
#define PROFILESWIDGET_H

#include <QDate>
#include <QWidget>

#include "ProfileEditWidget.h"
#include "ProfileModel.h"
#include "UserProfileController.h"

class QVBoxLayout;
class QStackedWidget;

class ProfilesWidget : public QWidget {
    Q_OBJECT

   public:
    explicit ProfilesWidget(QWidget *parent = nullptr);

    /**
     * @brief Destructor for the ProfilesWidget class.
     */
    ~ProfilesWidget();

    /**
     * @brief Sets the user profile controller.
     * @param controller The UserProfileController instance.
     */
    void setUserProfileController(UserProfileController *controller);

    /**
     * @brief Sets the current user ID.
     * @param userId The ID of the current user.
     */
    void setUserId(int userId);

    /**
     * @brief Loads the profiles for the current user.
     */
    void loadProfiles();

   signals:
    /**
     * @brief Emitted when a profile is selected.
     * @param profileId The ID of the selected profile.
     */
    void profileSelected(int profileId);

    /**
     * @brief Emitted when a new profile is requested.
     */
    void newProfileRequested();

    /**
     * @brief Emitted when profiles have changed (e.g., added, updated, or
     * deleted).
     */
    void profilesChanged();

   protected:
    /**
     * @brief Event filter to handle custom events.
     * @param obj The object receiving the event.
     * @param event The event being received.
     * @return True if the event was handled; otherwise false.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

   private slots:
    /**
     * @brief Shows the profile edit view.
     * @param profileId The ID of the profile to edit. If -1, a new profile is
     * created.
     */
    void showProfileEdit(int profileId = -1);

    /**
     * @brief Handles the action when returning from the edit view.
     */
    void handleBackFromEdit();

    /**
     * @brief Handles the profile save action.
     * @param name The name of the profile.
     * @param sex The sex of the profile.
     * @param weight The weight of the profile.
     * @param height The height of the profile.
     * @param dob The date of birth of the profile.
     */
    void handleProfileSave(QString name, QString sex, int weight, int height,
                           QDate dob);

    /**
     * @brief Handles the profile delete action.
     * @param profileId The ID of the profile to delete.
     */
    void handleProfileDelete(int profileId);

   private:
    /**
     * @brief Creates the list view of profiles.
     */
    void createListView();

    /**
     * @brief Creates the "New Profile" card.
     */
    void createNewProfileCard();

    /**
     * @brief Clears all profiles from the list.
     */
    void clearProfiles();

    /**
     * @brief Refreshes the profiles displayed in the list.
     */
    void refreshProfiles();

    /**
     * @brief Adds a profile to the list.
     * @param profile The ProfileModel instance to add.
     */
    void addProfile(ProfileModel *profile);

    /**
     * @brief Checks if the widget is being destroyed.
     * @return True if the widget is being destroyed; otherwise false.
     */
    bool isBeingDestroyed() const {
        return parent() == nullptr || parent()->parent() == nullptr;
    }

    // Members
    QStackedWidget *stackedWidget;
    QWidget *listWidget;
    ProfileEditWidget *editWidget;
    QVBoxLayout *profilesLayout;
    UserProfileController *profileController;
    int currentUserId;
    QVector<ProfileModel *> profiles;
};

#endif  // PROFILESWIDGET_H
