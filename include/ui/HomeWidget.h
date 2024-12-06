// HomeWidget.h
#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>

#include "UserProfileController.h"

class QComboBox;
class QLabel;

class HomeWidget : public QWidget {
    Q_OBJECT

   public:
    explicit HomeWidget(QWidget* parent, UserProfileController* controller);
    void setUserId(int userId);
    void refreshProfiles();
    QComboBox* getProfileSelector() const { return profileSelector; }

   signals:
    void profileSelected(int profileId, const QString& name);

   private:
    void setupUI();
    void populateProfileList();

    UserProfileController* profileController;
    QComboBox* profileSelector;
    QLabel* dateTimeLabel;
    QLabel* welcomeLabel;
    int currentUserId;
};

#endif  // HOMEWIDGET_H
