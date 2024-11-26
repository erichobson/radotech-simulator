/**
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class.
 */

#include "MainWindow.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <QPalette>
#include <QPixmap>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "BuyWidget.h"
#include "DatabaseManager.h"
#include "DeviceImageWidget.h"
#include "HistoryWidget.h"
#include "HomeWidget.h"
#include "LearningMaterialsWidget.h"
#include "LoginWidget.h"
#include "MeasureNowWidget.h"
#include "ProfileWidget.h"
#include "ProfilesWidget.h"
#include "SettingsWidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("RaDoTech");
    resize(1200, 800);

    // Initialize the database
    DatabaseManager db;

    // Create the main stacked widget
    stackedWidget = new QStackedWidget;
    setCentralWidget(stackedWidget);

    // Initialize the login widget
    loginWidget = new LoginWidget;

    // Connect signals from LoginWidget to MainWindow slots
    connect(loginWidget, &LoginWidget::loginRequested, this,
            &MainWindow::onLoginRequested);
    connect(loginWidget, &LoginWidget::registerRequested, this,
            &MainWindow::onRegisterRequested);

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
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);

    // TODO: Incorporate user name
    // Create the profile widget
    ProfileWidget *profileWidget =
        new ProfileWidget(":/images/dr.yoshio_nakatani.png", "User Name");
    sidebarLayout->addWidget(profileWidget);

    // Create the sidebar menu
    sidebarMenu = new QListWidget;
    sidebarMenu->setViewMode(QListView::ListMode);
    sidebarMenu->setSpacing(5);
    sidebarMenu->setUniformItemSizes(false);
    sidebarMenu->setSelectionMode(QAbstractItemView::SingleSelection);
    sidebarMenu->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sidebarMenu->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
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

    // Load icons and set title
    items = {
        {":/icons/measure_unselected.png", ":/icons/measure_selected.png",
         "Measure Now"},
        {":/icons/home_unselected.png", ":/icons/home_selected.png", "Home"},
        {":/icons/profiles_unselected.png", ":/icons/profiles_selected.png",
         "Profiles"},
        {":/icons/history_unselected.png", ":/icons/history_selected.png",
         "History"},
        {":/icons/buy_unselected.png", ":/icons/buy_selected.png", "Buy"},
        {":/icons/learning_unselected.png", ":/icons/learning_selected.png",
         "Learning Materials"},
        {":/icons/settings_unselected.png", ":/icons/settings_selected.png",
         "Settings"}};

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

    // Create final logout item
    QListWidgetItem *logoutItem = new QListWidgetItem;
    logoutItem->setText("Log Out");

    QFont logoutFont;
    logoutFont.setUnderline(true);
    logoutFont.setPointSize(14);
    logoutItem->setFont(logoutFont);
    logoutItem->setTextAlignment(Qt::AlignCenter);
    sidebarMenu->addItem(logoutItem);

    // Connect signals for navigation
    connect(sidebarMenu, &QListWidget::currentRowChanged, this,
            [this](int currentRow) {
                if (currentRow >= 0 && currentRow < items.size()) {
                    contentStackedWidget->setCurrentIndex(currentRow);
                } else if (currentRow == items.size()) {
                    logout();
                }
            });

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
    HomeWidget *homeWidget = new HomeWidget;
    ProfilesWidget *profilesWidget = new ProfilesWidget;
    HistoryWidget *historyWidget = new HistoryWidget;
    BuyWidget *buyWidget = new BuyWidget;
    LearningMaterialsWidget *learningMaterialsWidget =
        new LearningMaterialsWidget;
    SettingsWidget *settingsWidget = new SettingsWidget;

    // Add the widgets to the contentStackedWidget
    contentStackedWidget->addWidget(measureNowWidget);         // Index 0
    contentStackedWidget->addWidget(homeWidget);               // Index 1
    contentStackedWidget->addWidget(profilesWidget);           // Index 2
    contentStackedWidget->addWidget(historyWidget);            // Index 3
    contentStackedWidget->addWidget(buyWidget);                // Index 4
    contentStackedWidget->addWidget(learningMaterialsWidget);  // Index 5
    contentStackedWidget->addWidget(settingsWidget);           // Index 6

    // Create the device image widget
    QPixmap devicePixmap(":/images/radotech_device.png");
    QTransform tr;
    tr.rotate(270);
    devicePixmap = devicePixmap.transformed(tr, Qt::SmoothTransformation);

    DeviceImageWidget *deviceImageLabel = new DeviceImageWidget(devicePixmap);

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
    if (username == "user@domain.tld" && password == "password") {
        // Successful login: switch to mainWidget
        sidebarMenu->setCurrentRow(1);
        contentStackedWidget->setCurrentIndex(1);
        stackedWidget->setCurrentWidget(mainWidget);

        // Clear login fields
        loginWidget->clearFields();
    } else {
        // Show error message on LoginWidget
        loginWidget->setStatusMessage("Invalid username or password");
    }
}

void MainWindow::onRegisterRequested() {
    // TODO: Implement registration logic
}

void MainWindow::logout() {
    // Reset the selection to "Home"
    sidebarMenu->setCurrentRow(1);
    contentStackedWidget->setCurrentIndex(1);

    // Return to login page
    stackedWidget->setCurrentWidget(loginWidget);
}
