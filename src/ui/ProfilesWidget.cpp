/**
 * @file ProfilesWidget.cpp
 * @brief Implementation of the ProfilesWidget class.
 */

#include "ProfilesWidget.h"

#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QVariant>

#include "Logging.h"

/**
 * @brief Constructor for the ProfilesWidget class.
 * @param parent The parent widget.
 */
ProfilesWidget::ProfilesWidget(QWidget *parent)
    : QWidget(parent),
      currentUserId(-1),
      stackedWidget(nullptr),
      listWidget(nullptr),
      editWidget(nullptr),
      profilesLayout(nullptr),
      profileController(nullptr),
      profiles() {
    INFO("Initializing ProfilesWidget");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    stackedWidget = new QStackedWidget;
    if (!stackedWidget) {
        ERROR("Failed to create stackedWidget");
        return;
    }
    mainLayout->addWidget(stackedWidget);

    listWidget = new QWidget;
    if (!listWidget) {
        ERROR("Failed to create listWidget");
        return;
    }
    createListView();

    editWidget = new ProfileEditWidget;
    if (!editWidget) {
        ERROR("Failed to create ProfileEditWidget");
        return;
    }

    stackedWidget->addWidget(listWidget);
    stackedWidget->addWidget(editWidget);

    connect(editWidget, &ProfileEditWidget::backRequested, this,
            &ProfilesWidget::handleBackFromEdit);
    connect(editWidget, &ProfileEditWidget::saveRequested, this,
            &ProfilesWidget::handleProfileSave);
    connect(editWidget, &ProfileEditWidget::deleteRequested, this,
            &ProfilesWidget::handleProfileDelete);

    DEBUG("ProfilesWidget initialization complete");
}

/**
 * @brief Destructor for the ProfilesWidget class.
 */
ProfilesWidget::~ProfilesWidget() {
    DEBUG("ProfilesWidget destructor called");
    qDeleteAll(profiles);
    profiles.clear();
}

/**
 * @brief Clears all profile cards from the display and cleans up profiles.
 */
void ProfilesWidget::clearProfiles() {
    DEBUG("Starting profile cleanup");

    if (!profilesLayout) {
        ERROR("Profiles layout is null");
        return;
    }

    profilesLayout->blockSignals(true);

    // Preserve the "New Profile" card if it exists
    QWidget *newProfileCard = nullptr;
    if (profilesLayout->count() > 0) {
        QLayoutItem *item = profilesLayout->itemAt(0);
        if (item && item->widget() &&
            item->widget()->property("isNewProfileCard").toBool()) {
            DEBUG("Preserving New Profile card");
            newProfileCard = item->widget();
            profilesLayout->removeWidget(newProfileCard);
        }
    }

    // Remove and delete existing profile cards
    QLayoutItem *item;
    int removedCount = 0;
    while ((item = profilesLayout->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            if (widget != newProfileCard) {
                widget->removeEventFilter(this);
                widget->deleteLater();
                removedCount++;
            }
        }
        delete item;
    }
    DEBUG(QString("Removed %1 profile cards").arg(removedCount));

    qDeleteAll(profiles);
    profiles.clear();
    profilesLayout->blockSignals(false);

    // Restore or create the "New Profile" card
    if (newProfileCard) {
        profilesLayout->addWidget(newProfileCard);
    } else if (!this->isBeingDestroyed()) {
        createNewProfileCard();
    }

    profilesLayout->addStretch();
    INFO("Profile cleanup completed");
}

/**
 * @brief Creates the "New Profile" card and adds it to the profiles layout.
 */
void ProfilesWidget::createNewProfileCard() {
    DEBUG("Creating new profile card");

    if (!profilesLayout) {
        ERROR("profilesLayout is null");
        return;
    }

    // Create the card widget
    QWidget *card = new QWidget;
    if (!card) {
        ERROR("Failed to create new profile card widget");
        return;
    }

    card->setProperty("isNewProfileCard", true);
    card->setFixedHeight(80);
    card->setStyleSheet(
        "QWidget { background-color: white; border-radius: 10px; }");

    QHBoxLayout *cardLayout = new QHBoxLayout(card);

    QLabel *iconLabel = new QLabel;
    int picSize = 50;

    QPixmap profilePicPixmap(":/icons/add_profile.png");

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

        iconLabel->setPixmap(circularPixmap);
        iconLabel->setAlignment(Qt::AlignCenter);
    } else {
        // Fallback if image not available
        iconLabel->setFixedSize(picSize, picSize);
        iconLabel->setStyleSheet(
            QString("background-color: %1; border-radius: %2px;")
                .arg("#4ECBB1")
                .arg(picSize / 2));
    }

    QLabel *nameLabel = new QLabel("New Profile");
    nameLabel->setStyleSheet("color: #333333; font-size: 16px;");

    cardLayout->addWidget(iconLabel);
    cardLayout->addWidget(nameLabel);
    cardLayout->addStretch();

    card->setCursor(Qt::PointingHandCursor);
    card->installEventFilter(this);

    profilesLayout->addWidget(card);
    DEBUG("New profile card created successfully");
}

/**
 * @brief Adds a profile to the display.
 * @param profile The profile model to add.
 */
void ProfilesWidget::addProfile(ProfileModel *profile) {
    if (!profile) {
        WARNING("Null profile passed to addProfile");
        return;
    }

    DEBUG(QString("Adding profile: %1").arg(profile->getName()));

    if (!profilesLayout) {
        ERROR("Profiles layout is null");
        return;
    }

    QWidget *card = new QWidget;
    if (!card) {
        ERROR("Failed to create profile card widget");
        return;
    }

    card->setProperty("isProfileCard", true);
    card->setFixedHeight(80);
    card->setStyleSheet(
        "QWidget { background-color: white; border-radius: 10px; }");

    QHBoxLayout *cardLayout = new QHBoxLayout(card);

    QLabel *iconLabel = new QLabel;
    int picSize = 50;

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

        iconLabel->setPixmap(circularPixmap);
        iconLabel->setAlignment(Qt::AlignCenter);
    } else {
        // Placeholder circle if image not available
        iconLabel->setFixedSize(picSize, picSize);
        iconLabel->setStyleSheet(
            QString("background-color: #CCCCCC; border-radius: %1px;")
                .arg(picSize / 2));
    }

    QLabel *nameLabel = new QLabel(profile->getName());
    nameLabel->setStyleSheet("color: #333333; font-size: 16px;");

    cardLayout->addWidget(iconLabel);
    cardLayout->addWidget(nameLabel);
    cardLayout->addStretch();

    card->setCursor(Qt::PointingHandCursor);
    card->installEventFilter(this);
    card->setProperty("profile_id", QVariant(profile->getId()));

    int insertPosition = 1;
    if (profilesLayout->count() > 0 && !profilesLayout->itemAt(0)
                                            ->widget()
                                            ->property("isNewProfileCard")
                                            .toBool()) {
        insertPosition = 0;
    }
    profilesLayout->insertWidget(insertPosition, card);
    DEBUG("Profile card added successfully");
}

/**
 * @brief Creates the list view for displaying profiles.
 */
void ProfilesWidget::createListView() {
    DEBUG("Creating list view");

    if (!listWidget) {
        ERROR("listWidget is null");
        return;
    }

    QVBoxLayout *listLayout = new QVBoxLayout(listWidget);
    listLayout->setContentsMargins(0, 0, 0, 0);

    QScrollArea *scrollArea = new QScrollArea;
    if (!scrollArea) {
        ERROR("Failed to create scrollArea");
        return;
    }

    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet(
        "QScrollArea { background: transparent; }"
        "QScrollArea > QWidget > QWidget { background: transparent; }");

    QWidget *containerWidget = new QWidget;
    containerWidget->setStyleSheet("background: transparent;");

    profilesLayout = new QVBoxLayout(containerWidget);
    profilesLayout->setSpacing(10);
    profilesLayout->setContentsMargins(0, 0, 0, 0);

    createNewProfileCard();
    profilesLayout->addStretch();

    scrollArea->setWidget(containerWidget);
    listLayout->addWidget(scrollArea);

    DEBUG("List view created successfully");
}

/**
 * @brief Sets the user profile controller.
 * @param controller The UserProfileController to interact with.
 */
void ProfilesWidget::setUserProfileController(
    UserProfileController *controller) {
    DEBUG("Setting user profile controller");
    profileController = controller;
    if (currentUserId != -1) {
        loadProfiles();
    }
}

/**
 * @brief Sets the current user ID and loads profiles.
 * @param userId The ID of the current user.
 */
void ProfilesWidget::setUserId(int userId) {
    DEBUG(QString("Setting user ID: %1").arg(userId));
    currentUserId = userId;
    if (profileController) {
        loadProfiles();
    }
}

/**
 * @brief Loads the profiles for the current user.
 */
void ProfilesWidget::loadProfiles() {
    DEBUG(QString("Loading profiles for user ID: %1").arg(currentUserId));

    if (!profileController) {
        WARNING("Profile controller is null");
        return;
    }

    if (currentUserId == -1) {
        WARNING("No user ID set");
        return;
    }

    clearProfiles();

    QVector<ProfileModel *> loadedProfiles;
    if (profileController->getProfiles(currentUserId, loadedProfiles)) {
        DEBUG(QString("Successfully loaded %1 profiles")
                  .arg(loadedProfiles.size()));

        for (const ProfileModel *profile : loadedProfiles) {
            if (profile) {
                profiles.append(new ProfileModel(
                    profile->getId(), profile->getUserId(), profile->getName(),
                    profile->getDesc(), profile->getSex(), profile->getWeight(),
                    profile->getHeight(), profile->getDob()));
            }
        }

        qDeleteAll(loadedProfiles);
        loadedProfiles.clear();

        refreshProfiles();
        INFO("Profiles loaded successfully");
    } else {
        WARNING("Failed to load profiles");
    }
}

/**
 * @brief Refreshes the profiles display by rebuilding the profile cards.
 */
void ProfilesWidget::refreshProfiles() {
    DEBUG("Refreshing profiles display");

    if (!profilesLayout) {
        ERROR("Profiles layout is null");
        return;
    }

    if (profilesLayout->count() > 0) {
        QLayoutItem *lastItem =
            profilesLayout->takeAt(profilesLayout->count() - 1);
        delete lastItem;
    }

    for (ProfileModel *profile : profiles) {
        if (profile) {
            addProfile(profile);
        }
    }

    profilesLayout->addStretch();
    INFO("Profiles refreshed successfully");
}

/**
 * @brief Event filter to handle clicks on profile cards.
 * @param obj The object that received the event.
 * @param event The event that occurred.
 * @return True if the event was handled, otherwise false.
 */
bool ProfilesWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QWidget *card = qobject_cast<QWidget *>(obj);
        if (card) {
            QVariant profileId = card->property("profile_id");
            if (profileId.isNull()) {
                DEBUG("New profile card clicked");
                showProfileEdit(-1);
            } else {
                DEBUG(QString("Profile card clicked, ID: %1")
                          .arg(profileId.toInt()));
                showProfileEdit(profileId.toInt());
            }
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

/**
 * @brief Shows the profile edit widget for a given profile ID.
 * @param profileId The ID of the profile to edit. If -1, a new profile is
 * created.
 */
void ProfilesWidget::showProfileEdit(int profileId) {
    if (!editWidget || !stackedWidget) {
        ERROR("Edit widget or stacked widget is null");
        return;
    }

    editWidget->clearFields();

    if (profileId != -1) {
        for (ProfileModel *profile : profiles) {
            if (profile && profile->getId() == profileId) {
                DEBUG(QString("Editing profile: %1").arg(profile->getName()));
                editWidget->setProfile(profile);
                editWidget->setProperty("editing_profile_id", profileId);
                break;
            }
        }
    } else {
        DEBUG("Creating new profile");
        editWidget->setProperty("editing_profile_id", QVariant());
    }

    stackedWidget->setCurrentWidget(editWidget);
}

/**
 * @brief Handles the action when returning from the profile edit view.
 */
void ProfilesWidget::handleBackFromEdit() {
    DEBUG("Handling back from edit");
    if (!stackedWidget || !listWidget) {
        ERROR("Stacked widget or list widget is null");
        return;
    }

    stackedWidget->setCurrentWidget(listWidget);
}

/**
 * @brief Handles saving a profile from the edit widget.
 * @param name The name of the profile.
 * @param sex The sex of the profile.
 * @param weight The weight of the profile.
 * @param height The height of the profile.
 * @param dob The date of birth of the profile.
 */
void ProfilesWidget::handleProfileSave(QString name, QString sex, int weight,
                                       int height, QDate dob) {
    DEBUG(QString("Handling profile save - Name: %1, Sex: %2")
              .arg(name)
              .arg(sex));

    if (!profileController) {
        ERROR("Profile controller is null");
        return;
    }

    QString desc = "";
    bool success;
    QVariant profileIdVar = editWidget->property("editing_profile_id");

    if (profileIdVar.isValid()) {
        int profileId = profileIdVar.toInt();
        DEBUG(QString("Updating existing profile ID: %1").arg(profileId));
        success = profileController->updateProfile(
            profileId, currentUserId, name, desc, sex, weight, height, dob);
    } else {
        DEBUG("Creating new profile");
        success = profileController->createProfile(currentUserId, name, desc,
                                                   sex, weight, height, dob);
    }

    if (success) {
        INFO("Profile saved successfully");
        loadProfiles();
        handleBackFromEdit();
        emit profilesChanged();
    } else {
        WARNING("Failed to save profile");
    }
}

/**
 * @brief Handles profile deletion when the deleteRequested signal is emitted.
 * @param profileId The ID of the profile to delete.
 */
void ProfilesWidget::handleProfileDelete(int profileId) {
    if (profiles.size() <= 1) {
        WARNING("Cannot delete the last profile");
        handleBackFromEdit();
        return;
    }

    if (profileController->deleteProfile(profileId)) {
        INFO(QString("Profile %1 deleted successfully").arg(profileId));
        loadProfiles();
        handleBackFromEdit();
        emit profilesChanged();
    } else {
        WARNING(QString("Failed to delete profile %1").arg(profileId));
    }
}

/**
 * @brief
 *
 * @return
 */
ProfileModel *ProfilesWidget::getFirstProfile() const {
    if (profiles.isEmpty()) {
        return nullptr;
    }
    return profiles.first();
}
