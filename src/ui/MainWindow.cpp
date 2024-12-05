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
#include "DatabaseManager.h"
#include "DeviceImageWidget.h"
#include "HistoryWidget.h"
#include "HomeWidget.h"
#include "LearningMaterialsWidget.h"
#include "LoginWidget.h"
#include "MeasureNowWidget.h"
#include "ProfileWidget.h"
#include "ProfilesWidget.h"

/* Logging Macros */
#define DEBUG(msg) qDebug() << "[DEBUG]" << __FUNCTION__ << ":" << msg
#define INFO(msg) qInfo() << "[INFO]" << __FUNCTION__ << ":" << msg
#define ERROR(msg) qCritical() << "[ERROR]" << __FUNCTION__ << ":" << msg

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("RaDoTech");
    setMinimumSize(1200, 800);

    // Initialize the database
    DatabaseManager db;

    // Initialize controllers
    deviceController = new DeviceController(this);
    // ...
    // ...
    // ...
    // Initialize other controllers

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
         "History"},
        {":/icons/learning_unselected.png", ":/icons/learning_selected.png",
         "Learning Materials"}};

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
    MeasureNowWidget *measureNowWidget = new MeasureNowWidget;
    measureNowWidget->setDeviceController(deviceController);
    // TODO: Implement measureNowWidget interface

    HomeWidget *homeWidget = new HomeWidget;
    // TODO: Implement homeWidget interface

    ProfilesWidget *profilesWidget = new ProfilesWidget;
    // TODO: Implement profilesWidget interface

    HistoryWidget *historyWidget = new HistoryWidget;
    // TODO: Implement historyWidget interface

    LearningMaterialsWidget *learningMaterialsWidget =
        new LearningMaterialsWidget;
    // TODO: Implement learningMaterialsWidget interface

    // Add the widgets to the contentStackedWidget
    contentStackedWidget->addWidget(measureNowWidget);         // Index 0
    contentStackedWidget->addWidget(homeWidget);               // Index 1
    contentStackedWidget->addWidget(profilesWidget);           // Index 2
    contentStackedWidget->addWidget(historyWidget);            // Index 3
    contentStackedWidget->addWidget(learningMaterialsWidget);  // Index 4

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

    // Set stretch factors to control the sizes
    mainLayout->setStretch(0, 0);
    mainLayout->setStretch(1, 1);
    mainLayout->setStretch(2, 0);

    // Add loginWidget and mainWidget to stackedWidget
    stackedWidget->addWidget(loginWidget);
    stackedWidget->addWidget(mainWidget);

    // Set the default selected item in the sidebar menu
    sidebarMenu->setCurrentRow(1);
    contentStackedWidget->setCurrentIndex(1);

    // Show the login page initially
    stackedWidget->setCurrentIndex(0);

    // Connect signals from LoginWidget to MainWindow slots
    connect(loginWidget, &LoginWidget::loginRequested, this,
            &MainWindow::onLoginRequested);
    connect(loginWidget, &LoginWidget::registerRequested, this,
            &MainWindow::onRegisterRequested);

    // Connect signals for navigation
    connect(sidebarMenu, &QListWidget::currentRowChanged, this,
            [this](int currentRow) {
                if (currentRow >= 0 && currentRow < items.size()) {
                    contentStackedWidget->setCurrentIndex(currentRow);
                } else if (currentRow == items.size()) {
                    logout();
                }
            });

    // Connect device state changed signal
    connect(deviceController, &DeviceController::deviceStateChanged, this,
            [](bool isOn) { DEBUG("Device turned" << (isOn ? "on" : "off")); });

    connect(deviceImageLabel, &DeviceImageWidget::imageTouchingEdge,
            measureNowWidget, &MeasureNowWidget::startCountdown);
    connect(deviceImageLabel, &DeviceImageWidget::imageReleased,
            measureNowWidget, &MeasureNowWidget::onImageReleased);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    // Update the background gradient to match the new size
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor("#FF7009"));
    gradient.setColorAt(1, QColor("#FD2747"));

    QPalette pal = palette();
    pal.setBrush(QPalette::Window, QBrush(gradient));
    setPalette(pal);
}

void MainWindow::onLoginRequested(const QString &username,
                                  const QString &password) {
    // TODO: Replace this with proper authentication logic
    // if (username == "user@domain.tld" && password == "password") {
    //     // Successful login: switch to mainWidget
    //     sidebarMenu->setCurrentRow(1);
    //     contentStackedWidget->setCurrentIndex(1);
    //     stackedWidget->setCurrentWidget(mainWidget);
    //
    //     // Clear login fields
    //     loginWidget->clearFields();
    // } else {
    //     // Show error message on LoginWidget
    //     loginWidget->setStatusMessage("Invalid username or password");
    // }

    // -------- REMOVE AFTER TESTING --------
    sidebarMenu->setCurrentRow(1);
    contentStackedWidget->setCurrentIndex(1);
    stackedWidget->setCurrentWidget(mainWidget);
    loginWidget->clearFields();
    // --------------------------------------
}

void MainWindow::onRegisterRequested(const QString &firstName, const QString &lastName, const QString &gender,int weight, int height, const QDate &dob, const QString &email, const QString &password, const QString &confirmPassword) {
    // TODO: Implement registration logic
    qDebug() << "Main Windw handling emit:";
    qDebug() << "First Name:" << firstName;
    qDebug() << "Last Name:" << lastName;
    qDebug() << "Gender:" << gender;
    qDebug() << "Weight:" << weight;
    qDebug() << "Height:" << height;
    qDebug() << "Date of Birth:" << dob.toString("yyyy-MM-dd");
    qDebug() << "Email:" << email;
    qDebug() << "Password:" << password;
    qDebug() << "Confirm Password:" << confirmPassword;
}

void MainWindow::logout() {
    // Reset the selection to "Home"
    sidebarMenu->setCurrentRow(1);
    contentStackedWidget->setCurrentIndex(1);

    // Return to login page
    stackedWidget->setCurrentWidget(loginWidget);
}

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

void MainWindow::onBatteryPercentageClicked() {
    // Change the charging state
    if (deviceController->isCharging()) {
        deviceController->stopCharging();
    } else {
        deviceController->startCharging();
    }
}
