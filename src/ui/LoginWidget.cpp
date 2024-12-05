/**
 * @file LoginWidget.cpp
 * @brief Implementation of the LoginWidget class.
 */

#include "LoginWidget.h"

#include <QButtonGroup>
#include <QDateEdit>
#include <QDebug>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QPixmap>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedLayout>
#include <QVBoxLayout>

/**
 * @brief Constructor for the LoginWidget class.
 * @param parent The parent widget.
 */
LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent),
      buttonStyle(
          // Style template for buttons using placeholders for color and radius
          "QPushButton {"
          "    background-color: %1;"
          "    color: %2;"
          "    border-radius: %3px;"
          "    padding: 5px;"
          "    font-size: 16px;"
          "}"
          "QPushButton:hover {"
          "    background-color: #f0f0f0;"
          "}"
          "QPushButton:pressed {"
          "    background-color: #e0e0e0;"
          "}") {
    // Main layout for the login widget
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // QStackedLayout to switch between pages
    stackedLayout = new QStackedLayout;
    mainLayout->addLayout(stackedLayout);

    // Create different pages
    createInitialPage();
    createLoginPage();
    createRegistrationPage();

    // Set the initial page
    stackedLayout->setCurrentWidget(initialPage);

    // Set the background gradient
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor("#FF7009"));  // Orange
    gradient.setColorAt(1, QColor("#FD2747"));  // Red

    QPalette pal = palette();
    pal.setBrush(QPalette::Window, QBrush(gradient));
    setPalette(pal);
    setAutoFillBackground(true);

    // Set focus policy to prevent initial focus on input fields
    setFocusPolicy(Qt::StrongFocus);
}

/**
 * @brief Creates the initial page with the logo and initial buttons.
 */
void LoginWidget::createInitialPage() {
    initialPage = new QWidget(this);
    QVBoxLayout *initialLayout = new QVBoxLayout(initialPage);
    initialLayout->setSpacing(0);
    initialLayout->setContentsMargins(0, 0, 0, 0);

    // Add the logo
    QLabel *logoLabel = new QLabel;
    QPixmap logoPixmap(":/images/radotech_logo.png");
    if (!logoPixmap.isNull()) {
        // Scale and set the logo
        logoLabel->setPixmap(logoPixmap.scaled(200, 200, Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation));
        logoLabel->setAlignment(Qt::AlignCenter);
    } else {
        // Placeholder circle if image not available
        logoLabel->setAlignment(Qt::AlignCenter);
        logoLabel->setFixedSize(200, 200);
        logoLabel->setStyleSheet(
            "background-color: white; border-radius: 100px; width: 200px; "
            "height: 200px;");
    }

    // Create "Create profile" and "Enter" buttons
    createProfileButton = new QPushButton("Create profile");
    enterButton = new QPushButton("Enter");

    // Set button sizes
    int inputWidth = 200;
    int inputHeight = 40;
    createProfileButton->setFixedSize(inputWidth, inputHeight);
    enterButton->setFixedSize(inputWidth, inputHeight);

    // Apply styles to buttons
    createProfileButton->setStyleSheet(
        buttonStyle.arg("white").arg("#4ECBB1").arg(inputHeight / 2));
    enterButton->setStyleSheet(
        buttonStyle.arg("white").arg("#4ECBB1").arg(inputHeight / 2));

    // Connect signals to slots
    connect(createProfileButton, &QPushButton::clicked, this,
            &LoginWidget::onCreateProfileButtonClicked);
    connect(enterButton, &QPushButton::clicked, this,
            &LoginWidget::onEnterButtonClicked);

    // Arrange elements on the initial page
    initialLayout->addStretch();
    initialLayout->addWidget(logoLabel, 0, Qt::AlignCenter);
    initialLayout->addSpacing(40);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(20);
    buttonLayout->addWidget(createProfileButton);
    buttonLayout->addWidget(enterButton);

    initialLayout->addLayout(buttonLayout);
    initialLayout->addStretch();

    // Add the initial page to the stacked layout
    stackedLayout->addWidget(initialPage);
}

/**
 * @brief Creates the login page with email and password fields.
 */
void LoginWidget::createLoginPage() {
    loginPage = new QWidget(this);
    QVBoxLayout *loginLayout = new QVBoxLayout(loginPage);
    loginLayout->setSpacing(20);
    loginLayout->setContentsMargins(20, 20, 20, 20);

    // Back Button
    backButton = new QPushButton("Back");
    backButton->setFixedSize(60, 30);
    QString transparentButtonStyle =
        "QPushButton {"
        "    background-color: transparent;"
        "    color: white;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    color: #f0f0f0;"
        "}"
        "QPushButton:pressed {"
        "    color: #e0e0e0;"
        "}";
    backButton->setStyleSheet(transparentButtonStyle);
    connect(backButton, &QPushButton::clicked, this,
            &LoginWidget::onBackButtonClicked);

    // Add the logo
    QLabel *logoLabel = new QLabel;
    QPixmap logoPixmap(":/images/radotech_logo.png");
    if (!logoPixmap.isNull()) {
        // Scale and set the logo
        logoLabel->setPixmap(logoPixmap.scaled(150, 150, Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation));
        logoLabel->setAlignment(Qt::AlignCenter);
    } else {
        // Placeholder circle if image not available
        logoLabel->setAlignment(Qt::AlignCenter);
        logoLabel->setFixedSize(150, 150);
        logoLabel->setStyleSheet(
            "background-color: white; border-radius: 75px; width: 150px; "
            "height: 150px;");
    }

    // Initialize line edits for email and password
    emailLineEdit = new QLineEdit;
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    // Set placeholder text
    emailLineEdit->setPlaceholderText("Login (email)");
    passwordLineEdit->setPlaceholderText("Password");

    // Adjust focus policies to prevent automatic focus
    emailLineEdit->setFocusPolicy(Qt::ClickFocus);
    passwordLineEdit->setFocusPolicy(Qt::ClickFocus);

    // Set sizes for input fields
    int inputWidth = 200;
    int inputHeight = 40;
    emailLineEdit->setFixedSize(inputWidth, inputHeight);
    passwordLineEdit->setFixedSize(inputWidth, inputHeight);

    // Style for input fields
    QString lineEditStyle =
        "QLineEdit {"
        "    background-color: transparent;"
        "    border: none;"
        "    border-bottom: 1px solid white;"
        "    color: white;"
        "    padding: 5px;"
        "    border-radius: 0;"
        "}"
        "QLineEdit::placeholder {"
        "    color: #777777;"
        "}";
    emailLineEdit->setStyleSheet(lineEditStyle);
    passwordLineEdit->setStyleSheet(lineEditStyle);

    // Center align text in input fields
    emailLineEdit->setAlignment(Qt::AlignCenter);
    passwordLineEdit->setAlignment(Qt::AlignCenter);

    // Arrange input fields
    QVBoxLayout *inputLayout = new QVBoxLayout;
    inputLayout->addWidget(emailLineEdit);
    inputLayout->addWidget(passwordLineEdit);
    inputLayout->setAlignment(Qt::AlignCenter);
    inputLayout->setSpacing(10);

    // Status label for error messages
    statusLabel = new QLabel;
    statusLabel->setStyleSheet("QLabel { color: blue; }");
    statusLabel->setAlignment(Qt::AlignCenter);

    // "Remind password" and "Start" buttons
    remindPasswordButton = new QPushButton("Remind password");
    startButton = new QPushButton("Start");

    // Set button sizes
    remindPasswordButton->setFixedSize(inputWidth, inputHeight);
    startButton->setFixedSize(inputWidth, inputHeight);

    // Apply styles to buttons
    remindPasswordButton->setStyleSheet(transparentButtonStyle);
    startButton->setStyleSheet(
        buttonStyle.arg("white").arg("#4ECBB1").arg(inputHeight / 2));

    // Connect signals to slots
    connect(startButton, &QPushButton::clicked, this,
            &LoginWidget::onStartButtonClicked);
    connect(remindPasswordButton, &QPushButton::clicked, this,
            &LoginWidget::onRemindPasswordButtonClicked);
    connect(passwordLineEdit, &QLineEdit::returnPressed, startButton,
            &QPushButton::click);

    // Arrange all elements on the login page
    loginLayout->addWidget(backButton, 0, Qt::AlignLeft | Qt::AlignTop);
    loginLayout->addStretch();
    loginLayout->addWidget(logoLabel, 0, Qt::AlignCenter);
    loginLayout->addSpacing(20);
    loginLayout->addLayout(inputLayout);
    loginLayout->addSpacing(10);
    loginLayout->addWidget(statusLabel);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(10);
    buttonLayout->addWidget(remindPasswordButton);
    buttonLayout->addWidget(startButton);

    loginLayout->addLayout(buttonLayout);
    loginLayout->addStretch();

    // Add the login page to the stacked layout
    stackedLayout->addWidget(loginPage);
}

/**
 * @brief Creates the registration page.
 */
void LoginWidget::createRegistrationPage() {
    registrationPage = new QWidget(this);
    QVBoxLayout *registrationLayout = new QVBoxLayout(registrationPage);
    registrationLayout->setSpacing(20);
    registrationLayout->setContentsMargins(20, 20, 20, 20);

    // Back Button
    QPushButton *backButton = new QPushButton("Back");
    backButton->setFixedSize(60, 30);
    QString transparentButtonStyle =
        "QPushButton {"
        "    background-color: transparent;"
        "    color: white;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    color: #f0f0f0;"
        "}"
        "QPushButton:pressed {"
        "    color: #e0e0e0;"
        "}";
    backButton->setStyleSheet(transparentButtonStyle);
    connect(backButton, &QPushButton::clicked, this,
            &LoginWidget::onBackButtonClicked);

    registrationLayout->addWidget(backButton, 0, Qt::AlignLeft | Qt::AlignTop);
    registrationLayout->addStretch();

    // Card Layout
    QWidget *cardWidget = new QWidget;
    cardWidget->setStyleSheet("background-color: white; border-radius: 10px;");
    QVBoxLayout *cardLayout = new QVBoxLayout(cardWidget);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(20, 20, 20, 20);

    // Form Layout
    QWidget *formWidget = new QWidget;
    QGridLayout *formLayout = new QGridLayout(formWidget);
    formLayout->setContentsMargins(0, 0, 0, 0);
    formLayout->setHorizontalSpacing(10);
    formLayout->setVerticalSpacing(15);

    // Left Column Widgets (Labels and Profile Picture)
    QPixmap profilePicPixmap(":/images/dr.yoshio_nakatani.png");
    QLabel *profilePicLabel = new QLabel;

    if (!profilePicPixmap.isNull()) {
        // Scale the pixmap to desired size
        profilePicPixmap = profilePicPixmap.scaled(
            100, 100, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

        // Create a circular mask for the profile picture
        QPixmap circularPixmap(100, 100);
        circularPixmap.fill(Qt::transparent);

        QPainter painter(&circularPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(0, 0, 100, 100);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, profilePicPixmap);

        profilePicLabel->setPixmap(circularPixmap);
        profilePicLabel->setAlignment(Qt::AlignCenter);
    } else {
        // Placeholder circle if image not available
        profilePicLabel->setFixedSize(100, 100);
        profilePicLabel->setStyleSheet(
            "background-color: #CCCCCC; border-radius: 50px; width: 100px; "
            "height: 100px;");
    }

    // Labels for the form fields
    QLabel *weightLabel = new QLabel("Weight, kg");
    QLabel *heightLabel = new QLabel("Height, cm");
    QLabel *dobLabel = new QLabel("Date of Birth");
    QLabel *emailLabel = new QLabel("Email");
    QLabel *passwordLabel = new QLabel("Password");
    QLabel *confirmPasswordLabel = new QLabel("Confirm Password");

    // Style for labels
    QString labelStyle =
        "QLabel {"
        "    color: #333333;"
        "}";
    weightLabel->setStyleSheet(labelStyle);
    heightLabel->setStyleSheet(labelStyle);
    dobLabel->setStyleSheet(labelStyle);
    emailLabel->setStyleSheet(labelStyle);
    passwordLabel->setStyleSheet(labelStyle);
    confirmPasswordLabel->setStyleSheet(labelStyle);

    // Right Column Widgets (Input Fields)
    firstNameLineEdit = new QLineEdit;
    lastNameLineEdit = new QLineEdit;
    firstNameLineEdit->setPlaceholderText("Firstname");
    lastNameLineEdit->setPlaceholderText("Lastname");

    // Set focus policy to prevent initial focus
    firstNameLineEdit->setFocusPolicy(Qt::ClickFocus);
    lastNameLineEdit->setFocusPolicy(Qt::ClickFocus);

    // Radio Buttons
    maleRadioButton = new QRadioButton("Male");
    femaleRadioButton = new QRadioButton("Female");

    // Group radio buttons
    sexButtonGroup = new QButtonGroup(this);
    sexButtonGroup->addButton(maleRadioButton);
    sexButtonGroup->addButton(femaleRadioButton);
    sexButtonGroup->setExclusive(true);

    // Style for radio buttons
    QString radioButtonStyle =
        "QRadioButton {"
        "    color: #333333;"
        "    font-size: 12px;"
        "}";
    maleRadioButton->setStyleSheet(radioButtonStyle);
    femaleRadioButton->setStyleSheet(radioButtonStyle);

    // Layout for sex selection
    QHBoxLayout *sexLayout = new QHBoxLayout;
    sexLayout->addWidget(maleRadioButton);
    sexLayout->addWidget(femaleRadioButton);
    sexLayout->addStretch();

    // Line Edits corresponding to labels
    weightLineEdit = new QLineEdit;
    heightLineEdit = new QLineEdit;
    dobDateEdit = new QDateEdit;
    dobDateEdit->setDisplayFormat("MM-dd-yyyy");
    dobDateEdit->setDate(QDate::currentDate());

    // Set focus policy to prevent initial focus
    weightLineEdit->setFocusPolicy(Qt::ClickFocus);
    heightLineEdit->setFocusPolicy(Qt::ClickFocus);
    dobDateEdit->setFocusPolicy(Qt::ClickFocus);

    // Style for QDateEdit
    QString dateEditStyle =
        "QDateEdit {"
        "    background-color: transparent;"
        "    border: none;"
        "    border-bottom: 1px solid orange;"
        "    color: #333333;"
        "    padding: 5px;"
        "    border-radius: 0;"
        "}"
        "QDateEdit::drop-down {"
        "    border: none;"
        "}"
        "QDateEdit::down-arrow {"
        "    image: none;"
        "}";
    dobDateEdit->setStyleSheet(dateEditStyle);

    emailRegLineEdit = new QLineEdit;
    passwordRegLineEdit = new QLineEdit;
    confirmPasswordLineEdit = new QLineEdit;

    // Set placeholder text
    weightLineEdit->setPlaceholderText("Your weight");
    heightLineEdit->setPlaceholderText("Your height");
    emailRegLineEdit->setPlaceholderText("Login");
    passwordRegLineEdit->setPlaceholderText("12345");
    confirmPasswordLineEdit->setPlaceholderText("12345");

    // Set focus policy to prevent initial focus
    emailRegLineEdit->setFocusPolicy(Qt::ClickFocus);
    passwordRegLineEdit->setFocusPolicy(Qt::ClickFocus);
    confirmPasswordLineEdit->setFocusPolicy(Qt::ClickFocus);

    // Set echo mode for password fields
    passwordRegLineEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);

    // Style for input fields
    QString inputFieldStyle =
        "QLineEdit {"
        "    background-color: transparent;"
        "    border: none;"
        "    border-bottom: 1px solid orange;"
        "    color: #333333;"
        "    padding: 5px;"
        "    border-radius: 0;"
        "}"
        "QLineEdit::placeholder {"
        "    color: #777777;"
        "}";
    firstNameLineEdit->setStyleSheet(inputFieldStyle);
    lastNameLineEdit->setStyleSheet(inputFieldStyle);
    weightLineEdit->setStyleSheet(inputFieldStyle);
    heightLineEdit->setStyleSheet(inputFieldStyle);
    emailRegLineEdit->setStyleSheet(inputFieldStyle);
    passwordRegLineEdit->setStyleSheet(inputFieldStyle);
    confirmPasswordLineEdit->setStyleSheet(inputFieldStyle);

    // Arrange widgets in the grid layout
    // Left column (1/5 width)
    formLayout->addWidget(profilePicLabel, 0, 0, 3, 1, Qt::AlignTop);
    formLayout->addWidget(weightLabel, 3, 0);
    formLayout->addWidget(heightLabel, 4, 0);
    formLayout->addWidget(dobLabel, 5, 0);
    formLayout->addWidget(emailLabel, 6, 0);
    formLayout->addWidget(passwordLabel, 7, 0);
    formLayout->addWidget(confirmPasswordLabel, 8, 0);

    // Right column (4/5 width)
    formLayout->addWidget(firstNameLineEdit, 0, 1);
    formLayout->addWidget(lastNameLineEdit, 1, 1);
    formLayout->addLayout(sexLayout, 2, 1);
    formLayout->addWidget(weightLineEdit, 3, 1);
    formLayout->addWidget(heightLineEdit, 4, 1);
    formLayout->addWidget(dobDateEdit, 5, 1);
    formLayout->addWidget(emailRegLineEdit, 6, 1);
    formLayout->addWidget(passwordRegLineEdit, 7, 1);
    formLayout->addWidget(confirmPasswordLineEdit, 8, 1);

    // Set column stretch factors
    formLayout->setColumnStretch(0, 1);
    formLayout->setColumnStretch(1, 4);

    // "Save and continue" Button
    QPushButton *saveContinueButton = new QPushButton("Save and continue");
    saveContinueButton->setFixedSize(250, 40);
    QString saveButtonStyle =
        "QPushButton {"
        "    background-color: #FF7009;"
        "    color: white;"
        "    border-radius: 20px;"
        "    padding: 5px;"
        "    font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #E66008;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #CC5A07;"
        "}";
    saveContinueButton->setStyleSheet(saveButtonStyle);

    // Connect signal to slot
    connect(saveContinueButton, &QPushButton::clicked, this,
            &LoginWidget::onSaveContinueButtonClicked);

    // Add form and button to card layout
    cardLayout->addWidget(formWidget);
    cardLayout->addWidget(saveContinueButton, 0, Qt::AlignCenter);

    // Add card to registration layout
    registrationLayout->addWidget(cardWidget);
    registrationLayout->addStretch();

    // Add the registration page to the stacked layout
    stackedLayout->addWidget(registrationPage);
}

/**
 * @brief Slot called when the "Enter" button is clicked.
 *        Switches to the login page.
 */
void LoginWidget::onEnterButtonClicked() {
    // Switch to the login page
    stackedLayout->setCurrentWidget(loginPage);

    // Set focus to the login page to prevent input fields from getting focus
    loginPage->setFocus();
}

/**
 * @brief Slot called when the "Back" button is clicked.
 *        Clears fields and returns to the initial page.
 */
void LoginWidget::onBackButtonClicked() {
    // Reset fields
    clearFields();

    // Switch back to the initial page
    stackedLayout->setCurrentWidget(initialPage);

    // Set focus to the initial page
    initialPage->setFocus();
}

/**
 * @brief Slot called when the "Create profile" button is clicked.
 *        Switches to the registration page.
 */
void LoginWidget::onCreateProfileButtonClicked() {
    // Switch to the registration page
    stackedLayout->setCurrentWidget(registrationPage);

    // Set focus to the registration page
    registrationPage->setFocus();
}

/**
 * @brief Slot called when the "Save and continue" button is clicked.
 *        Emits a signal indicating the profile has been created.
 */
void LoginWidget::onSaveContinueButtonClicked() {

    QString firstName = firstNameLineEdit->text();
    QString lastName = lastNameLineEdit->text();
    QAbstractButton* selected = sexButtonGroup->checkedButton();
    QString sex = selected == nullptr ? "" : selected->text();
    QString weight = weightLineEdit->text();
    QString height = heightLineEdit->text();
    QDate dob = dobDateEdit == nullptr ? QDate() : dobDateEdit->date();
    QString email = emailRegLineEdit->text();
    QString password = passwordRegLineEdit->text();
    QString confirmPassword = confirmPasswordLineEdit->text();

    // Verify name
    if(firstName.isEmpty() || lastName.isEmpty()) {
        registrationStatusLabel->setText("Enter first and last name");
        return;
    }

    // Verify sex, weight, and height
    if(sex.isEmpty() || weight.isEmpty() || height.isEmpty()) {
        registrationStatusLabel->setText("Enter sex, weight, and height");
        return;
    }

    // Verify weight and height only contain numbers
    if(!(weight.contains(QRegularExpression("^[0-9]+$"))) || !(height.contains(QRegularExpression("^[0-9]+$")))) {
        registrationStatusLabel->setText("Weight and height can only contain numbers");
        return;
    } 

    // Verfiy dob
    if(dob.isNull()){
        registrationStatusLabel->setText("Enter your date of birth (MM-dd-yyyy)");
        return;
    }

    // Verify the format of the email
    const QRegularExpression regex(R"(^[^\s@]+@[^\s@]+\.[^\s@]+$)");
    QRegularExpressionMatch match = regex.match(email.trimmed());
    if (email.isEmpty() || !match.hasMatch()) {
         registrationStatusLabel->setText("Enter email");
         return;
    }

    // Verify password
    if(password.isEmpty()) {
        registrationStatusLabel->setText("Enter password");
    }

    // Verify confirm passowrd
    if(confirmPassword.isEmpty()) {
        registrationStatusLabel->setText("Confirm password");
    }

    // Verify passwords match
    if(!(confirmPassword == password)){
        registrationStatusLabel->setText("Passwords do not match");
    }

    emit registerRequested(firstName, lastName, sex, weight, height, dob, email, password, confirmPassword);

    // Emit the profileCreated signal
    //emit profileCreated();
}

/**
 * @brief Slot called when the "Start" button is clicked.
 *        Validates input and emits a signal to request login.
 */
void LoginWidget::onStartButtonClicked() {
    QString email = emailLineEdit->text();
    QString password = passwordLineEdit->text();

    // Verify the format of the email
    const QRegularExpression regex(R"(^[^\s@]+@[^\s@]+\.[^\s@]+$)");
    QRegularExpressionMatch match = regex.match(email.trimmed());
    if (email.isEmpty() || !match.hasMatch()) {
         statusLabel->setText("Enter email");
         return;
    }
    
    // Verify password is not empty
    if (password.isEmpty()) {
        statusLabel->setText("Enter password");
        return;
    }

    // Emit the loginRequested signal
    emit loginRequested(email, password);
}

/**
 * @brief Slot called when the "Remind Password" button is clicked.
 *        Emits a signal to handle password reminder.
 */
void LoginWidget::onRemindPasswordButtonClicked() {
    // TODO: Implement functionality

    // Emit the remindPasswordRequested signal
    emit remindPasswordRequested();
}

/**
 * @brief Clears the input fields and status messages.
 */
void LoginWidget::clearFields() {
    // Clear fields on the login page
    if (emailLineEdit) {
        emailLineEdit->clear();
    }
    if (passwordLineEdit) {
        passwordLineEdit->clear();
    }
    if (statusLabel) {
        statusLabel->clear();
    }

    // Clear fields on the registration page
    if (firstNameLineEdit) {
        firstNameLineEdit->clear();
    }
    if (lastNameLineEdit) {
        lastNameLineEdit->clear();
    }
    if (sexButtonGroup) {
        sexButtonGroup->setExclusive(false);
        foreach (QAbstractButton *button, sexButtonGroup->buttons()) {
            button->setChecked(false);
        }
        sexButtonGroup->setExclusive(true);
    }
    if (weightLineEdit) {
        weightLineEdit->clear();
    }
    if (heightLineEdit) {
        heightLineEdit->clear();
    }
    if (dobDateEdit) {
        dobDateEdit->setDate(QDate::currentDate());
    }
    if (emailRegLineEdit) {
        emailRegLineEdit->clear();
    }
    if (passwordRegLineEdit) {
        passwordRegLineEdit->clear();
    }
    if (confirmPasswordLineEdit) {
        confirmPasswordLineEdit->clear();
    }
    if (registrationStatusLabel) {
        registrationStatusLabel->clear();
    }
}

/**
 * @brief Sets the status message displayed on the widget.
 * @param message The message to display.
 */
void LoginWidget::setStatusMessage(const QString &message) {
    statusLabel->setText(message);
}

/**
 * @brief Sets the status message displayed on the registration page.
 * @param message The message to display.
 */
void LoginWidget::setRegistrationStatusMessage(const QString &message) {
    registrationStatusLabel->setText(message);
}
