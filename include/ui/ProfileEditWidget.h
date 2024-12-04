/**
 * @file ProfileEditWidget.h
 * @brief Declaration of the ProfileEditWidget class.
 */

#ifndef PROFILEEDITWIDGET_H
#define PROFILEEDITWIDGET_H

#include <QDate>
#include <QWidget>

#include "ProfileModel.h"

class QButtonGroup;
class QDateEdit;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;

class ProfileEditWidget : public QWidget {
    Q_OBJECT

   public:
    explicit ProfileEditWidget(QWidget *parent = nullptr);

    /**
     * @brief Sets the profile data into the edit fields.
     * @param profile The ProfileModel instance to set.
     */
    void setProfile(ProfileModel *profile);

    /**
     * @brief Clears all input fields and resets the form.
     */
    void clearFields();

   signals:
    /**
     * @brief Emitted when the back action is requested.
     */
    void backRequested();

    /**
     * @brief Emitted when the delete action is requested.
     * @param profileId The ID of the profile to delete.
     */
    void deleteRequested(int profileId);

    /**
     * @brief Emitted when the save action is requested.
     * @param name The name of the profile.
     * @param sex The sex of the profile.
     * @param weight The weight of the profile.
     * @param height The height of the profile.
     * @param dob The date of birth of the profile.
     */
    void saveRequested(QString name, QString sex, int weight, int height,
                       QDate dob);

   private:
    /**
     * @brief Creates the layout for the widget.
     */
    void createLayout();

    /**
     * @brief Sets up the styles for the widget components.
     */
    void setupStyles();

    // Members
    QLabel *profilePicLabel;
    QPushButton *deleteButton;
    QLineEdit *nameLineEdit;
    QRadioButton *maleRadioButton;
    QRadioButton *femaleRadioButton;
    QButtonGroup *sexButtonGroup;
    QLineEdit *weightLineEdit;
    QLineEdit *heightLineEdit;
    QDateEdit *dobDateEdit;
};

#endif  // PROFILEEDITWIDGET_H
