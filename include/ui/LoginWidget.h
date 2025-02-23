/**
 * @file LoginWidget.h
 * @brief Declaration of the LoginWidget class.
 */

#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

class QPushButton;
class QButtonGroup;
class QLineEdit;
class QLabel;
class QStackedLayout;
class QRadioButton;
class QDateEdit;

class LoginWidget : public QWidget {
    Q_OBJECT
   public:
    explicit LoginWidget(QWidget *parent = nullptr);

    /**
     * @brief Clears the input fields and status message.
     */
    void clearFields();

    /**
     * @brief Sets the status message displayed on the widget.
     * @param message The message to display.
     */
    void setStatusMessage(const QString &message);

    /**
     * @brief Sets the status message displayed on the registration page.
     * @param message The message to display.
     */
    void setRegistrationStatusMessage(const QString &message);

   signals:
    void loginRequested(const QString &email, const QString &password);
    //void registerRequested();
    void remindPasswordRequested();
    void profileCreated();
    void registerRequested(const QString &firstName, const QString &lastName, const QString &sex, const QString &weight, 
                           const QString &height, const QDate &dob, const QString &email, const QString& password, 
                           const QString &confirmPassword);

   private slots:
    void onEnterButtonClicked();
    void onBackButtonClicked();
    void onStartButtonClicked();
    void onRemindPasswordButtonClicked();
    void onCreateProfileButtonClicked();
    void onSaveContinueButtonClicked();

   private:
    void createInitialPage();
    void createLoginPage();
    void createRegistrationPage();

    // Layouts
    QStackedLayout *stackedLayout;

    // Initial Page Widgets
    QWidget *initialPage;
    QPushButton *createProfileButton;
    QPushButton *enterButton;

    // Login Page Widgets
    QWidget *loginPage;
    QLineEdit *emailLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *startButton;
    QPushButton *remindPasswordButton;
    QPushButton *backButton;
    QLabel *statusLabel;

    // Registration Page Widgets
    QWidget *registrationPage;
    QLineEdit *firstNameLineEdit;
    QLineEdit *lastNameLineEdit;
    QRadioButton *maleRadioButton;
    QRadioButton *femaleRadioButton;
    QButtonGroup *sexButtonGroup;
    QLineEdit *weightLineEdit;
    QLineEdit *heightLineEdit;
    QDateEdit *dobDateEdit;
    QLineEdit *emailRegLineEdit;
    QLineEdit *passwordRegLineEdit;
    QLineEdit *confirmPasswordLineEdit;
    QLabel *registrationStatusLabel;

    // Common Styles
    QString buttonStyle;
};

#endif  // LOGINWIDGET_H
