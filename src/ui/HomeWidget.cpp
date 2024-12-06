/**
 * @file HomeWidget.cpp
 * @brief
 */

#include "HomeWidget.h"

#include <QComboBox>
#include <QDateTime>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>

#include "BackgroundDelegate.h"
#include "Logging.h"
#include "ProfileModel.h"

HomeWidget::HomeWidget(QWidget* parent, UserProfileController* controller)
    : QWidget(parent), profileController(controller), currentUserId(-1) {
    DEBUG("Initializing HomeWidget");
    if (!controller) {
        ERROR("Null controller provided to HomeWidget");
    }
    setupUI();
}

/**
 * @brief
 */
void HomeWidget::setupUI() {
    DEBUG("Setting up HomeWidget UI");

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(30);

    auto* topContainer = new QWidget;
    auto* topLayout = new QVBoxLayout(topContainer);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(8);

    welcomeLabel = new QLabel("Welcome");
    welcomeLabel->setStyleSheet(
        "font-size: 42px;"
        "font-weight: bold;"
        "color: #333333;");

    auto* projectLabel =
        new QLabel("Team 38  •  RaDoTech Health Monitoring Device Simulation");
    projectLabel->setStyleSheet(
        "font-size: 16px;"
        "color: #999999;");

    auto* teamLabel = new QLabel(
        "Eric Hobson  •  Andrew Wallace  •  Olu Ogunmeru  •  Abdulmalik Umar");
    teamLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #999999;");

    topLayout->addWidget(welcomeLabel);
    topLayout->addWidget(projectLabel);
    topLayout->addWidget(teamLabel);

    auto* bottomContainer = new QWidget;
    auto* bottomLayout = new QVBoxLayout(bottomContainer);
    bottomLayout->setSpacing(15);
    bottomLayout->setContentsMargins(0, 0, 0, 0);

    profileSelector = new QComboBox;
    profileSelector->setStyleSheet(COMBOBOX_STYLE);
    profileSelector->setItemDelegate(new BackgroundDelegate(profileSelector));

    connect(profileSelector,
            QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this](int index) {
                if (index >= 0) {
                    int profileId = profileSelector->itemData(index).toInt();
                    QString name = profileSelector->itemText(index);
                    emit profileSelected(profileId, name);
                    DEBUG(QString("Profile selected - ID: %1, Name: %2")
                              .arg(profileId)
                              .arg(name));
                }
            });

    dateTimeLabel = new QLabel;
    dateTimeLabel->setStyleSheet(
        "font-size: 16px;"
        "color: #666666;");

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        QDateTime current = QDateTime::currentDateTime();
        dateTimeLabel->setText(
            current.toString("dddd, MMMM d, yyyy  |  hh:mm:ss ap"));
    });
    timer->start(1000);

    bottomLayout->addWidget(profileSelector);
    bottomLayout->addWidget(dateTimeLabel);

    mainLayout->addStretch();
    mainLayout->addWidget(topContainer);
    mainLayout->addWidget(bottomContainer);

    DEBUG("HomeWidget UI setup complete");
}

/**
 * @brief
 */
void HomeWidget::refreshProfiles() {
    INFO(QString("Refreshing profiles - Controller: %1, UserId: %2")
             .arg(profileController != nullptr ? "valid" : "null")
             .arg(currentUserId));
    if (profileController && currentUserId != -1) {
        populateProfileList();
    } else {
        WARNING("Cannot refresh profiles - invalid controller or userId");
    }
}

/**
 * @brief
 *
 * @param userId
 */
void HomeWidget::setUserId(int userId) {
    INFO(QString("Setting userId: %1").arg(userId));
    currentUserId = userId;
    if (userId != -1) {
        refreshProfiles();
    } else {
        WARNING("Received invalid userId");
    }
}

/**
 * @brief
 */
void HomeWidget::populateProfileList() {
    INFO("Starting profile population");

    if (!profileSelector || !profileController) {
        ERROR("Selector or controller is null");
        return;
    }

    profileSelector->clear();
    QVector<ProfileModel*> profiles;

    INFO(QString("Attempting to get profiles for userId: %1")
             .arg(currentUserId));
    if (profileController->getProfiles(currentUserId, profiles)) {
        INFO(QString("Retrieved %1 profiles").arg(profiles.size()));
        for (const auto* profile : profiles) {
            if (profile) {
                profileSelector->addItem(profile->getName(), profile->getId());
                INFO(QString("Added profile - Name: %1, ID: %2")
                         .arg(profile->getName())
                         .arg(profile->getId()));
            }
        }

        if (profileSelector->count() > 0) {
            profileSelector->setCurrentIndex(0);
            INFO("Set initial profile selection");
        } else {
            WARNING("No profiles were added to selector");
        }
    } else {
        ERROR("Failed to get profiles from controller");
    }

    qDeleteAll(profiles);
    profiles.clear();
}
