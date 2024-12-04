/**
 * @file ProfileEditWidget.cpp
 * @brief Implementation of the ProfileEditWidget class.
 */

#include "ProfileEditWidget.h"

#include <QButtonGroup>
#include <QDateEdit>
#include <QDebug>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#include "Logging.h"

/**
 * @brief Constructor for the ProfileEditWidget class.
 * @param parent The parent widget.
 */
ProfileEditWidget::ProfileEditWidget(QWidget *parent) : QWidget(parent) {
    DEBUG("Initializing ProfileEditWidget");
    createLayout();
    setupStyles();
    INFO("ProfileEditWidget initialized successfully");
}

/**
 * @brief Creates the layout for the ProfileEditWidget.
 */
void ProfileEditWidget::createLayout() {
    DEBUG("Creating ProfileEditWidget layout");

    // Main layout for the widget
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(0, 20, 0, 20);

    // Card Layout
    QWidget *cardWidget = new QWidget;
    cardWidget->setStyleSheet("background-color: white; border-radius: 10px;");
    QVBoxLayout *cardLayout = new QVBoxLayout(cardWidget);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(40, 20, 40, 20);

    // Back Button to return to the profiles list
    QPushButton *backButton = new QPushButton("Back", this);
    backButton->setFixedSize(60, 30);
    connect(backButton, &QPushButton::clicked, this,
            &ProfileEditWidget::backRequested);

    // Profile Picture Container
    QWidget *profilePicContainer = new QWidget;
    QVBoxLayout *profilePicLayout = new QVBoxLayout(profilePicContainer);

    // Profile Picture
    profilePicLabel = new QLabel;
    int picSize = 120;

    // Load the profile picture
    QPixmap profilePicPixmap(":/images/default_profile.png");

    if (!profilePicPixmap.isNull()) {
        // Scale the pixmap to desired size
        profilePicPixmap = profilePicPixmap.scaled(
            picSize, picSize, Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation);

        // Create a circular mask for the profile picture
        QPixmap circularPixmap(picSize, picSize);
        circularPixmap.fill(Qt::transparent);

        QPainter painter(&circularPixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(0, 0, picSize, picSize);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, profilePicPixmap);

        profilePicLabel->setPixmap(circularPixmap);
        profilePicLabel->setAlignment(Qt::AlignCenter);
    } else {
        // Placeholder circle if image not available
        profilePicLabel->setFixedSize(picSize, picSize);
        profilePicLabel->setStyleSheet(
            QString("background-color: #CCCCCC; border-radius: %1px;")
                .arg(picSize / 2));
    }

    profilePicLayout->addWidget(profilePicLabel, 0, Qt::AlignCenter);

    // Form Layout for profile details
    QWidget *formWidget = new QWidget;
    QFormLayout *formLayout = new QFormLayout(formWidget);
    formLayout->setContentsMargins(0, 0, 0, 0);
    formLayout->setSpacing(15);
    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    // Style for form labels
    QString labelStyle = "QLabel { color: #333333; font-size: 14px; }";

    // Form Fields

    // Name Field
    nameLineEdit = new QLineEdit;
    nameLineEdit->setPlaceholderText("Name");
    QLabel *nameLabel = new QLabel("Name:");
    nameLabel->setStyleSheet(labelStyle);

    // Sex Selection Radio Buttons
    QWidget *sexWidget = new QWidget;
    QHBoxLayout *sexLayout = new QHBoxLayout(sexWidget);
    sexLayout->setContentsMargins(0, 0, 0, 0);

    maleRadioButton = new QRadioButton("Male");
    femaleRadioButton = new QRadioButton("Female");
    sexButtonGroup = new QButtonGroup(this);
    sexButtonGroup->addButton(maleRadioButton);
    sexButtonGroup->addButton(femaleRadioButton);

    sexLayout->addWidget(maleRadioButton);
    sexLayout->addWidget(femaleRadioButton);
    sexLayout->addStretch();

    QLabel *sexLabel = new QLabel("Sex:");
    sexLabel->setStyleSheet(labelStyle);

    // Weight Field
    weightLineEdit = new QLineEdit;
    weightLineEdit->setPlaceholderText("Weight in kg");
    QLabel *weightLabel = new QLabel("Weight (kg):");
    weightLabel->setStyleSheet(labelStyle);

    // Height Field
    heightLineEdit = new QLineEdit;
    heightLineEdit->setPlaceholderText("Height in cm");
    QLabel *heightLabel = new QLabel("Height (cm):");
    heightLabel->setStyleSheet(labelStyle);

    // Date of Birth Field
    dobDateEdit = new QDateEdit;
    dobDateEdit->setDisplayFormat("yyyy-MM-dd");
    dobDateEdit->setDate(QDate::currentDate());
    QLabel *dobLabel = new QLabel("Date of Birth:");
    dobLabel->setStyleSheet(labelStyle);

    // Add fields to form layout
    formLayout->addRow(nameLabel, nameLineEdit);
    formLayout->addRow(sexLabel, sexWidget);
    formLayout->addRow(weightLabel, weightLineEdit);
    formLayout->addRow(heightLabel, heightLineEdit);
    formLayout->addRow(dobLabel, dobDateEdit);

    // Add a stretch between the form fields and the buttons
    cardLayout->addWidget(backButton, 0, Qt::AlignLeft);
    cardLayout->addWidget(profilePicContainer);
    cardLayout->addWidget(formWidget);

    // Add stretch to push buttons to the bottom
    cardLayout->addStretch();

    // Delete Button to delete the profile
    deleteButton = new QPushButton("Delete Profile");
    deleteButton->setFixedSize(250, 40);
    deleteButton->setEnabled(false);

    connect(deleteButton, &QPushButton::clicked, this, [this]() {
        QVariant profileId = property("editing_profile_id");
        if (profileId.isValid()) {
            emit deleteRequested(profileId.toInt());
        }
    });

    // Save Button to save profile details
    QPushButton *saveButton = new QPushButton("Save");
    saveButton->setFixedSize(250, 40);
    connect(saveButton, &QPushButton::clicked, this, [this]() {
        DEBUG("Save button clicked - validating inputs");

        // Validate inputs before emitting
        if (nameLineEdit->text().isEmpty()) {
            WARNING("Name field is empty");
            return;
        }

        bool ok;
        int weight = weightLineEdit->text().toInt(&ok);
        if (!ok) {
            WARNING("Invalid weight value entered");
            return;
        }

        int height = heightLineEdit->text().toInt(&ok);
        if (!ok) {
            WARNING("Invalid height value entered");
            return;
        }

        if (!maleRadioButton->isChecked() && !femaleRadioButton->isChecked()) {
            WARNING("Sex not selected");
            return;
        }

        QString sex = maleRadioButton->isChecked() ? "Male" : "Female";
        DEBUG(QString("Emitting saveRequested - Name: %1, Sex: %2, Weight: %3, "
                      "Height: %4")
                  .arg(nameLineEdit->text())
                  .arg(sex)
                  .arg(weight)
                  .arg(height));

        emit saveRequested(nameLineEdit->text(), sex, weight, height,
                           dobDateEdit->date());
    });

    // Assemble the layout

    // Add delete button
    cardLayout->addWidget(deleteButton, 0, Qt::AlignCenter);

    // Add save button
    cardLayout->addWidget(saveButton, 0, Qt::AlignCenter);

    // Add card to main layout
    mainLayout->addWidget(cardWidget);

    DEBUG("ProfileEditWidget layout created successfully");
}

/**
 * @brief Sets up the styles for the ProfileEditWidget components.
 */
void ProfileEditWidget::setupStyles() {
    DEBUG("Setting up ProfileEditWidget styles");

    // Style for input fields
    QString inputFieldStyle =
        "QLineEdit {"
        "    background-color: transparent;"
        "    border: none;"
        "    border-bottom: 2px solid #E0E0E0;"
        "    padding: 5px;"
        "    color: #333333;"
        "    border-radius: 0px;"
        "}"
        "QLineEdit:focus {"
        "    border-bottom: 2px solid #FF7009;"
        "}";

    // Style for QDateEdit
    QString dateEditStyle =
        "QDateEdit {"
        "    background-color: transparent;"
        "    border: none;"
        "    border-bottom: 2px solid #E0E0E0;"
        "    padding: 5px;"
        "    color: #333333;"
        "    border-radius: 0px;"
        "}"
        "QDateEdit:focus {"
        "    border-bottom: 2px solid #FF7009;"
        "}"
        "QDateEdit::drop-down {"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QDateEdit::down-arrow {"
        "    image: none;"
        "}";

    // Style for radio buttons
    QString radioButtonStyle =
        "QRadioButton {"
        "    color: #333333;"
        "    font-size: 14px;"
        "}";

    // Style for the save button
    QString saveButtonStyle =
        "QPushButton {"
        "    background-color: #FF7009;"
        "    color: white;"
        "    border-radius: 20px;"
        "    font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #E66008;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #CC5A07;"
        "}";

    // Style for the delete button
    QString deleteButtonStyle =
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #DC3545;"
        "    font-size: 16px;"
        "    border: none;"
        "}"
        "QPushButton:disabled {"
        "    color: #AAAAAA;"
        "}"
        "QPushButton:hover:!disabled {"
        "    color: #C82333;"
        "}"
        "QPushButton:pressed:!disabled {"
        "    color: #BD2130;"
        "}";

    // Apply styles to input fields
    nameLineEdit->setStyleSheet(inputFieldStyle);
    weightLineEdit->setStyleSheet(inputFieldStyle);
    heightLineEdit->setStyleSheet(inputFieldStyle);
    dobDateEdit->setStyleSheet(dateEditStyle);

    // Apply styles to radio buttons
    maleRadioButton->setStyleSheet(radioButtonStyle);
    femaleRadioButton->setStyleSheet(radioButtonStyle);

    // Apply styles to buttons
    deleteButton->setStyleSheet(deleteButtonStyle);

    // Find and style specific buttons
    foreach (QPushButton *btn, findChildren<QPushButton *>()) {
        if (btn->text() == "Back") {
            // Style for the back button
            btn->setStyleSheet(
                "QPushButton {"
                "    background-color: transparent;"
                "    color: #333333;"
                "    font-size: 14px;"
                "    border: none;"
                "}"
                "QPushButton:hover {"
                "    color: #666666;"
                "}");
        } else if (btn->text() == "Save") {
            // Style for the save button
            btn->setStyleSheet(saveButtonStyle);
        }
    }

    DEBUG("Styles applied successfully");
}

/**
 * @brief Sets the profile data into the edit fields.
 * @param profile The profile model to set.
 */
void ProfileEditWidget::setProfile(ProfileModel *profile) {
    if (!profile) {
        WARNING("Attempted to set null profile");
        return;
    }

    DEBUG(QString("Setting profile data - Name: %1, Sex: %2")
              .arg(profile->getName())
              .arg(profile->getSex()));

    // Set the profile data into the fields
    nameLineEdit->setText(profile->getName());

    if (profile->getSex() == "Male" || profile->getSex() == "male")
        maleRadioButton->setChecked(true);
    else
        femaleRadioButton->setChecked(true);

    weightLineEdit->setText(QString::number(profile->getWeight()));
    heightLineEdit->setText(QString::number(profile->getHeight()));
    dobDateEdit->setDate(profile->getDob());

    // Enable the delete button when editing an existing profile
    deleteButton->setEnabled(true);
    INFO("Profile data set successfully");
}

/**
 * @brief Clears all input fields and resets the form.
 */
void ProfileEditWidget::clearFields() {
    DEBUG("Clearing all profile fields");

    // Clear all input fields
    nameLineEdit->clear();
    sexButtonGroup->setExclusive(false);
    maleRadioButton->setChecked(false);
    femaleRadioButton->setChecked(false);
    sexButtonGroup->setExclusive(true);
    weightLineEdit->clear();
    heightLineEdit->clear();
    dobDateEdit->setDate(QDate::currentDate());

    // Disable the delete button when creating a new profile
    deleteButton->setEnabled(false);

    DEBUG("All fields cleared successfully");
}
