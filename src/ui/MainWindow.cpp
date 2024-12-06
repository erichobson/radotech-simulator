/**
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class.
 */

#include "MainWindow.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <QPalette>
#include <QPixmap>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "ClickableLabel.h"
#include "DeviceImageWidget.h"
#include "HistoryWidget.h"
#include "HomeWidget.h"
#include "Logging.h"
#include "LoginWidget.h"
#include "MeasureNowWidget.h"
#include "ProfileWidget.h"
#include "ProfilesWidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("RaDoTech");
    setMinimumSize(1200, 800);

    // Initialize the database
    loggedInUserId = -1;
    currentProfileId = -1;
    currentProfileName = "";

    // Initialize controllers
    databaseManager = new DatabaseManager();
    deviceController = new DeviceController(this);
    userProfileController = new UserProfileController(*databaseManager);
    scanController = new ScanController(*databaseManager);
    userController = new UserController(*databaseManager);

    // Create the main stacked widget
    stackedWidget = new QStackedWidget;
    setCentralWidget(stackedWidget);

    // Initialize the login widget
    loginWidget = new LoginWidget;

    // Create the main application interface widget
    mainWidget = new QWidget;

    // Set up the main window background gradient
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor("#FF7009"));
    gradient.setColorAt(1, QColor("#FD2747"));

    // Apply gradient to the main window
    QPalette pal = palette();
    pal.setBrush(QPalette::Window, QBrush(gradient));
    setPalette(pal);
    setAutoFillBackground(true);

    // Create the main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    // Create the sidebar
    QWidget *sidebarWidget = new QWidget;
    sidebarWidget->setFixedWidth(250);
    sidebarWidget->setStyleSheet("background: transparent;");

    // Create the sidebar layout
    sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);

    // TODO: Incorporate user name
    // Create the profile widget
    ProfileWidget *profileWidget =
        new ProfileWidget(":/images/dr.yoshio_nakatani.png", "User Name");
    profileWidget->setFixedHeight(200);
    sidebarLayout->addWidget(profileWidget);

    // Create the sidebar menu
    sidebarMenu = new QListWidget;
    sidebarMenu->setViewMode(QListView::ListMode);
    sidebarMenu->setSpacing(5);
    sidebarMenu->setUniformItemSizes(false);
    sidebarMenu->setSelectionMode(QAbstractItemView::SingleSelection);
    sidebarMenu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sidebarMenu->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sidebarMenu->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sidebarMenu->setIconSize(QSize(24, 24));
    sidebarMenu->setStyleSheet(R"(
        QListWidget {
            background: transparent;
            border: none;
            outline: 0;
        }
        QListWidget::item {
            padding: 10px;
            margin: 0;
            color: white;
            border: none;
        }
        QListWidget::item:selected {
            background-color: white;
            border-radius: 15px;
            color: #333333;
        }
        QListWidget::item:hover {
            background-color: rgba(255, 255, 255, 0.1);
            border-radius: 15px;
            color: white;
        }
        QListWidget::item:selected:hover {
            background-color: white;
            border-radius: 15px;
            color: #333333;
        }
    )");

    // Add sidebar menu to sidebar layout
    sidebarLayout->addWidget(sidebarMenu);
    sidebarLayout->addStretch();

    setupBatteryInfoWidget();

    // Load icons and set title
    items = {
        {":/icons/measure_unselected.png", ":/icons/measure_selected.png",
         "Measure Now"},
        {":/icons/home_unselected.png", ":/icons/home_selected.png", "Home"},
        {":/icons/profiles_unselected.png", ":/icons/profiles_selected.png",
         "Profiles"},
        {":/icons/history_unselected.png", ":/icons/history_selected.png",
         "History"}};

    // Set font size for sidebar items
    QFont normalFont;
    normalFont.setPointSize(14);

    for (const ItemInfo &itemInfo : items) {
        QListWidgetItem *item = new QListWidgetItem;

        // Create the QIcon with different states
        QIcon icon;
        icon.addFile(itemInfo.iconPathUnselected, QSize(24, 24), QIcon::Normal,
                     QIcon::Off);
        icon.addFile(itemInfo.iconPathSelected, QSize(24, 24), QIcon::Selected,
                     QIcon::Off);
        icon.addFile(itemInfo.iconPathSelected, QSize(24, 24), QIcon::Active,
                     QIcon::Off);

        item->setIcon(icon);
        item->setText(itemInfo.title);
        item->setFont(normalFont);

        sidebarMenu->addItem(item);
    }

    // Set the fixed height of the sidebar menu
    sidebarMenu->setFixedHeight(525);

    // Create final logout item
    QListWidgetItem *logoutItem = new QListWidgetItem;
    logoutItem->setText("Log Out");

    QFont logoutFont;
    logoutFont.setUnderline(true);
    logoutFont.setPointSize(14);
    logoutItem->setFont(logoutFont);
    logoutItem->setTextAlignment(Qt::AlignCenter);
    sidebarMenu->addItem(logoutItem);

    // Create a card widget for the content area
    QWidget *contentCardWidget = new QWidget;
    contentCardWidget->setStyleSheet(
        "background-color: white; border-radius: 10px;");
    QVBoxLayout *contentCardLayout = new QVBoxLayout(contentCardWidget);
    contentCardLayout->setContentsMargins(20, 20, 20, 20);
    contentCardLayout->setSpacing(10);

    // Create a stacked widget for the content
    contentStackedWidget = new QStackedWidget;
    contentCardLayout->addWidget(contentStackedWidget);

    // Create the content widgets
    measureNowWidget =
        new MeasureNowWidget(this, deviceController, userProfileController,
                             scanController, loggedInUserId);

    homeWidget = new HomeWidget(this, userProfileController);
    homeWidget->setUserId(loggedInUserId);

    profilesWidget = new ProfilesWidget;
    profilesWidget->setUserProfileController(userProfileController);
    profilesWidget->setStyleSheet("background-color: transparent;");
    profilesWidget->setUserId(loggedInUserId);

    historyWidget =
        new HistoryWidget(this, userProfileController);
    historyWidget->setStyleSheet("background-color: transparent;");

    // Add the widgets to the contentStackedWidget
    contentStackedWidget->addWidget(measureNowWidget);  // Index 0
    contentStackedWidget->addWidget(homeWidget);        // Index 1
    contentStackedWidget->addWidget(profilesWidget);    // Index 2
    contentStackedWidget->addWidget(historyWidget);     // Index 3

    connect(contentStackedWidget, &QStackedWidget::currentChanged,
            [contentCardWidget](int index) {
                if (index == 2 || index == 3) {
                    contentCardWidget->setStyleSheet(
                        "background-color: transparent; border-radius: 10px;");
                } else {
                    contentCardWidget->setStyleSheet(
                        "background-color: white; border-radius: 10px;");
                }
            });

    // Create the device image widget
    QPixmap devicePixmap(":/images/radotech_device.png");
    QTransform tr;
    tr.rotate(270);
    devicePixmap = devicePixmap.transformed(tr, Qt::SmoothTransformation);
    DeviceImageWidget *deviceImageLabel = new DeviceImageWidget(devicePixmap);

    // Set the DeviceController
    deviceImageLabel->setDeviceController(deviceController);

    // Create a card widget for the device image
    QWidget *deviceCardWidget = new QWidget;
    deviceCardWidget->setStyleSheet(
        "background-color: white; border-radius: 10px;");
    QVBoxLayout *deviceCardLayout = new QVBoxLayout(deviceCardWidget);
    deviceCardLayout->setSpacing(20);
    deviceCardLayout->setContentsMargins(20, 20, 20, 20);
    deviceCardLayout->addWidget(deviceImageLabel);

    // Add widgets to main layout
    mainLayout->addWidget(sidebarWidget);
    mainLayout->addWidget(contentCardWidget);
    mainLayout->addWidget(deviceCardWidget);
    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 1);
    mainLayout->setStretch(2, 0);

    stackedWidget->addWidget(loginWidget);
    stackedWidget->addWidget(mainWidget);

    // Set the default selected item in the sidebar menu
    sidebarMenu->setCurrentRow(1);
    contentStackedWidget->setCurrentIndex(1);
    stackedWidget->setCurrentIndex(0);

    connect(loginWidget, &LoginWidget::loginRequested, this,
            &MainWindow::onLoginRequested);
    connect(loginWidget, &LoginWidget::registerRequested, this,
            &MainWindow::onRegisterRequested);

    connect(sidebarMenu, &QListWidget::currentRowChanged, this,
            [this](int currentRow) {
                if (currentRow >= 0 && currentRow < items.size()) {
                    contentStackedWidget->setCurrentIndex(currentRow);
                } else if (currentRow == items.size()) {
                    logout();
                }
            });

    connect(profilesWidget, &ProfilesWidget::profilesChanged, this, [this]() {
        if (ProfileModel *firstProfile = profilesWidget->getFirstProfile()) {
            setCurrentProfile(firstProfile->getId(), firstProfile->getName());
        }
    });

    connect(profilesWidget, &ProfilesWidget::profilesChanged, homeWidget,
            &HomeWidget::refreshProfiles);

    connect(homeWidget, &HomeWidget::profileSelected, this,
            &MainWindow::setCurrentProfile);

    connect(deviceController, &DeviceController::deviceStateChanged, this,
            [](bool isOn) { DEBUG("Device turned" << (isOn ? "on" : "off")); });

    connect(deviceImageLabel, &DeviceImageWidget::imageTouchingEdge,
            measureNowWidget, &MeasureNowWidget::startCountdown);
    connect(deviceImageLabel, &DeviceImageWidget::imageReleased,
            measureNowWidget, &MeasureNowWidget::onImageReleased);
    connect(profilesWidget, &ProfilesWidget::profilesChanged, measureNowWidget,
            &MeasureNowWidget::refreshProfiles);

    connect(measureNowWidget, &MeasureNowWidget::profileSelected, this,
            &MainWindow::setCurrentProfile);

    connect(this, &MainWindow::currentProfileChanged, historyWidget,
            [this](int profileId, const QString &) {
                historyWidget->setCurrentProfile(profileId);
            });

    if (ProfileModel *firstProfile = profilesWidget->getFirstProfile()) {
        setCurrentProfile(firstProfile->getId(), firstProfile->getName());
    }
}

/**
 * @brief
 *
 * @param event
 */
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor("#FF7009"));
    gradient.setColorAt(1, QColor("#FD2747"));

    QPalette pal = palette();
    pal.setBrush(QPalette::Window, QBrush(gradient));
    setPalette(pal);
}

/**
 * @brief Verifies and logs in a user. Sets the logged in user id.
 *
 * @param username the username (email) to verify
 * @param password the password to verify
 */
void MainWindow::onLoginRequested(const QString &username,
                                  const QString &password) {
    UserModel user;
    if(userController->validateUser(username, password, user)){
        // Successful login: switch to mainWidget
        sidebarMenu->setCurrentRow(1);
        contentStackedWidget->setCurrentIndex(1);
        stackedWidget->setCurrentWidget(mainWidget);
        loginWidget->clearFields();
        loggedInUserId = user.getId();
        homeWidget->setUserId(loggedInUserId);
        measureNowWidget->setUserId(loggedInUserId);
        profilesWidget->setUserId(loggedInUserId);
        DEBUG(QString("User Logged in: ID=%1, First Name=%2, Email=%3")
                  .arg(user.getId())
                  .arg(user.getFirstName())
                  .arg(user.getEmail())
        );

        // Get user's default profile
        QVector<ProfileModel*> profiles;
        if(userController->getUserProfiles(user.getId(), profiles)) {
            if(!profiles.isEmpty()) {
                ProfileModel *profile = profiles.at(0);
                DEBUG(QString("User Profile found: ID=%1, UserId=%2, Name=%3")
                  .arg(profile->getId())
                  .arg(profile->getUserId())
                  .arg(profile->getName())
                );
                if(profile != nullptr) {
                    setCurrentProfile(profile->getId(), profile->getName());
                }
            }
        }
        for(auto* p : profiles) delete p;

    } else {

        loginWidget->setStatusMessage("Invalid username or password");
    }
}

/**
 * @brief Verifies and creates a new user and default profile.
 * @param firstName the first name of the user
 * @param lastName the last name of the user
 * @param sex the sex of the user
 * @param weight the weight of the user
 * @param height the height of the user
 * @param dob the date of birth of the user
 * @param email the email of the user
 * @param password the password of the user
 * @param confirmPassword the confirmed password of the user
 */
void MainWindow::onRegisterRequested(const QString &firstName, const QString &lastName, const QString &sex, const QString &weight, 
                           const QString &height, const QDate &dob, const QString &email, const QString& password, 
                           const QString &confirmPassword){

    UserModel user;
    if(password != confirmPassword){
        loginWidget->setRegistrationStatusMessage("Passwords do not match");
        return;
    }
    if(userController->validateUser(email, password, user)){
        loginWidget->setRegistrationStatusMessage("User with that username/email already exists");
        return;
    }
    if(!userController->createUser(firstName, lastName, email, password, user)) {
        loginWidget->setRegistrationStatusMessage("Error registering the user");
        return;
    }
    
    DEBUG(QString("User Logged in: ID=%1, First Name=%2, Email=%3")
                  .arg(user.getId())
                  .arg(user.getFirstName())
                  .arg(user.getEmail())
    );

    if(!userProfileController->createProfile(user.getId(), firstName+ " " + lastName, "", sex, weight.toInt(), height.toInt(), dob)) {
        loginWidget->setRegistrationStatusMessage("Error creating profile");
        return;
    }

    // Successful login & profile creation: switch to mainWidget
    sidebarMenu->setCurrentRow(1);
    contentStackedWidget->setCurrentIndex(1);
    stackedWidget->setCurrentWidget(mainWidget);
    loginWidget->clearFields();
    loggedInUserId = user.getId(); 
    homeWidget->setUserId(loggedInUserId);
    measureNowWidget->setUserId(loggedInUserId);
    profilesWidget->setUserId(loggedInUserId);

    DEBUG(QString("User Logged in: ID=%1, First Name=%2, Email=%3")
                  .arg(user.getId())
                  .arg(user.getFirstName())
                  .arg(user.getEmail())
    );
    
    // Get user's default profile
    QVector<ProfileModel*> profiles;
    if(userController->getUserProfiles(user.getId(), profiles)) {
        if(!profiles.isEmpty()) {
            ProfileModel *profile = profiles.at(0);
            if(profile != nullptr) {
                DEBUG(QString("User Profile found: ID=%1, UserId=%2, Name=%3")
                  .arg(profile->getId())
                  .arg(profile->getUserId())
                  .arg(profile->getName())
                );
                setCurrentProfile(profile->getId(), profile->getName());
            }
        }
    }
    for(auto* p : profiles) delete p;
}

/**
 * @brief
 */
void MainWindow::logout() {
    // Reset the selection to "Home"
    sidebarMenu->setCurrentRow(1);
    contentStackedWidget->setCurrentIndex(1);

    // Return to login page
    stackedWidget->setCurrentWidget(loginWidget);
}

/**
 * @brief
 */
void MainWindow::setupBatteryInfoWidget() {
    // Create the container widget
    QFrame *batteryInfoFrame = new QFrame;
    batteryInfoFrame->setFixedHeight(30);
    batteryInfoFrame->setStyleSheet(
        "background-color: white; border-radius: 15px;");

    // Create the connection status label
    connectionStatusLabel = new QLabel("Disconnected");
    connectionStatusLabel->setStyleSheet(
        "font-size: 12px; color: red; background-color: transparent;");
    connectionStatusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    connectionStatusLabel->setSizePolicy(QSizePolicy::Expanding,
                                         QSizePolicy::Fixed);

    // Create the battery percentage label
    batteryPercentageLabel = new ClickableLabel();
    batteryPercentageLabel->setText("100%");
    batteryPercentageLabel->setStyleSheet(
        "font-size: 12px; color: black; background-color: transparent;");
    batteryPercentageLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    batteryPercentageLabel->setFixedWidth(40);
    batteryPercentageLabel->setSizePolicy(QSizePolicy::Fixed,
                                          QSizePolicy::Fixed);

    // Create the layout for batteryInfoFrame
    QHBoxLayout *batteryLayout = new QHBoxLayout(batteryInfoFrame);
    batteryLayout->setContentsMargins(15, 5, 15, 5);
    batteryLayout->setSpacing(10);

    // Add widgets to the layout
    batteryLayout->addWidget(connectionStatusLabel);
    batteryLayout->addWidget(batteryPercentageLabel);

    // Add the battery info frame to the sidebar layout
    sidebarLayout->addWidget(batteryInfoFrame);

    // Connect signals to update the battery percentage and connection status
    connect(deviceController, &DeviceController::connectionStatusChanged, this,
            &MainWindow::updateConnectionStatus);
    connect(deviceController, &DeviceController::batteryLevelChanged, this,
            &MainWindow::updateBatteryPercentage);
    connect(batteryPercentageLabel, &ClickableLabel::clicked, this,
            &MainWindow::onBatteryPercentageClicked);

    // Initialize the labels with the current device state
    updateConnectionStatus(deviceController->isConnected());
    updateBatteryPercentage(deviceController->getBatteryLevel());
}

/**
 * @brief
 *
 * @param isConnected
 */
void MainWindow::updateConnectionStatus(bool isConnected) {
    // Update the connection status label
    if (isConnected) {
        connectionStatusLabel->setText("Connected");
        connectionStatusLabel->setStyleSheet(
            "font-size: 12px; color: green; background-color: "
            "transparent;");
    } else {
        connectionStatusLabel->setText("Disconnected");
        connectionStatusLabel->setStyleSheet(
            "font-size: 12px; color: red; background-color: transparent;");
    }
}

/**
 * @brief
 *
 * @param batteryLevel
 */
void MainWindow::updateBatteryPercentage(int batteryLevel) {
    // Update the battery percentage text
    QString percentageText = QString("%1%").arg(batteryLevel);
    batteryPercentageLabel->setText(percentageText);

    // Update the battery percentage label and colours
    QString color;
    if (deviceController->isCharging()) {
        // Update the connection status label to charging
        connectionStatusLabel->setText("Charging");
        connectionStatusLabel->setStyleSheet(
            "font-size: 12px; color: green; background-color: transparent;");

        // Set colour to green
        color = "green";
    } else {
        // Update the connection status label when no longer charging
        updateConnectionStatus(deviceController->isConnected());

        // Set colour according to battery level
        if (batteryLevel >= 21) {
            color = "black";
        } else {
            color = "red";
        }
    }

    // Update the stylesheet to change the text color
    batteryPercentageLabel->setStyleSheet(
        QString("font-size: 12px; color: %1; background-color: transparent;")
            .arg(color));
}

/**
 * @brief
 */
void MainWindow::onBatteryPercentageClicked() {
    // Change the charging state
    if (deviceController->isCharging()) {
        deviceController->stopCharging();
    } else {
        deviceController->startCharging();
    }
}

/**
 * @brief
 *
 * @param profileId
 * @param profileName
 */
void MainWindow::setCurrentProfile(int profileId, const QString &profileName) {
    DEBUG(QString("Setting current profile: ID=%1, Name=%2")
              .arg(profileId)
              .arg(profileName));

    currentProfileId = profileId;
    currentProfileName = profileName;

    QComboBox *homeCombo = homeWidget->getProfileSelector();
    QComboBox *measureCombo = measureNowWidget->getProfileSelector();

    if (homeCombo) {
        int index = homeCombo->findData(profileId);
        if (index >= 0) homeCombo->setCurrentIndex(index);
    }

    if (measureCombo) {
        int index = measureCombo->findData(profileId);
        if (index >= 0) measureCombo->setCurrentIndex(index);
    }

    emit currentProfileChanged(profileId, profileName);
}
