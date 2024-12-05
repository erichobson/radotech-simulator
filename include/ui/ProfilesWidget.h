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
    ~ProfilesWidget();

    void setUserProfileController(UserProfileController *controller);
    void setUserId(int userId);
    void loadProfiles();

    ProfileModel *getFirstProfile() const;
    QVector<ProfileModel *> getAllProfiles() const { return profiles; }

   signals:
    void profileSelected(int profileId);
    void newProfileRequested();
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
    void showProfileEdit(int profileId = -1);
    void handleBackFromEdit();
    void handleProfileSave(QString name, QString sex, int weight, int height,
                           QDate dob);
    void handleProfileDelete(int profileId);

   private:
    void createListView();
    void createNewProfileCard();
    void clearProfiles();
    void refreshProfiles();
    void addProfile(ProfileModel *profile);
    bool isBeingDestroyed() const {
        return parent() == nullptr || parent()->parent() == nullptr;
    }

    int currentUserId;
    QStackedWidget *stackedWidget;
    QWidget *listWidget;
    ProfileEditWidget *editWidget;
    QVBoxLayout *profilesLayout;
    UserProfileController *profileController;
    QVector<ProfileModel *> profiles;
};

#endif  // PROFILESWIDGET_H
