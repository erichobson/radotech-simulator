/**
 * @file HistoryWidget.cpp
 * @brief
 */

#include "HistoryWidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "Logging.h"
#include "UserProfileController.h"

HistoryWidget::HistoryWidget(QWidget* parent, UserProfileController* controller)
    : QWidget(parent),
      userProfileController(controller),
      historyView(new QWidget(this)),
      resultsView(new ResultsWidget(this)),
      mainLayout(new QVBoxLayout(this)),
      scansGrid(new QVBoxLayout()),
      scrollArea(new QScrollArea(this)) {
    setupUI();
}

/**
 * @brief
 *
 * @param profileId
 */
void HistoryWidget::setCurrentProfile(int profileId) {
    DEBUG(QString("Setting current profile ID: %1").arg(profileId));

    qDeleteAll(profileScans);
    profileScans.clear();

    QLayoutItem* child;
    while ((child = scansGrid->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    if (profileId <= 0) {
        WARNING("Invalid profile ID");
        return;
    }

    currentProfileId = profileId;
    loadScansForProfile();
    showHistoryView();
}

/**
 * @brief
 */
void HistoryWidget::showHistoryView() {
    DEBUG("Showing history view");

    if (QLayoutItem* item = mainLayout->itemAt(0)) {
        if (item->widget()->property("isBackButton").toBool()) {
            delete item->widget();
        }
    }

    resultsView->hide();
    scrollArea->show();
}

/**
 * @brief
 *
 * @param scan
 */
void HistoryWidget::onScanSelected(ScanModel* scan) {
    if (!profileScans.contains(scan)) {
        WARNING("Attempted to access invalid scan");
        return;
    }

    DEBUG("Scan selected");
    showResultsView(scan);
}

/**
 * @brief
 */
void HistoryWidget::loadScansForProfile() {
    DEBUG(QString("Loading scans for profile ID: %1").arg(currentProfileId));

    qDeleteAll(profileScans);
    profileScans.clear();

    if (!userProfileController) {
        ERROR("UserProfileController is null");
        return;
    }

    if (currentProfileId <= 0) {
        WARNING("Invalid profile ID");
        return;
    }

    if (!userProfileController->getProfileScans(currentProfileId,
                                                profileScans)) {
        ERROR("Failed to load scans from database");
        return;
    }

    INFO(QString("Loaded %1 scans for profile").arg(profileScans.size()));

    if (profileScans.isEmpty()) {
        INFO("No scans found for this profile");

        QWidget* card = new QWidget;
        card->setStyleSheet(
            "QWidget { "
            "    background-color: white; "
            "    border-radius: 10px; "
            "    border: 1px solid #E0E0E0; "
            "}");

        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(20, 30, 20, 30);
        cardLayout->setSpacing(8);

        QLabel* messageLabel = new QLabel("No Scans Available");
        messageLabel->setStyleSheet(
            "background-color: transparent; "
            "color: #333333; "
            "font-size: 20px; "
            "font-weight: bold; "
            "border: none;");
        messageLabel->setAlignment(Qt::AlignCenter);

        QLabel* descLabel = new QLabel("Complete a new scan to see it here");
        descLabel->setStyleSheet(
            "background-color: transparent; "
            "color: #666666; "
            "font-size: 15px; "
            "border: none;");
        descLabel->setAlignment(Qt::AlignCenter);

        cardLayout->addWidget(messageLabel);
        cardLayout->addWidget(descLabel);

        scansGrid->addStretch();
        scansGrid->addWidget(card);
        scansGrid->addStretch();
        return;
    }

    displayScans();
}

/**
 * @brief
 */
void HistoryWidget::setupUI() {
    DEBUG("Setting up History UI");

    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet(
        "QScrollArea { background: transparent; }"
        "QScrollArea > QWidget > QWidget { background: transparent; }");

    QWidget* containerWidget = new QWidget;
    containerWidget->setStyleSheet("background: transparent;");

    scansGrid = new QVBoxLayout(containerWidget);
    scansGrid->setSpacing(10);
    scansGrid->setContentsMargins(0, 0, 0, 0);

    scrollArea->setWidget(containerWidget);
    mainLayout->addWidget(scrollArea);

    resultsView->hide();
    mainLayout->addWidget(resultsView);
}

/**
 * @brief
 */
void HistoryWidget::displayScans() {
    DEBUG("Displaying scans");

    QLayoutItem* child;
    while ((child = scansGrid->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    for (ScanModel* scan : profileScans) {
        QWidget* card = new QWidget;
        card->setFixedHeight(80);
        card->setObjectName("scanCard");

        QHBoxLayout* cardLayout = new QHBoxLayout(card);
        cardLayout->setContentsMargins(20, 0, 20, 0);
        cardLayout->setSpacing(20);

        QWidget* dateWidget = new QWidget;
        dateWidget->setStyleSheet("border: none;");
        QVBoxLayout* dateLayout = new QVBoxLayout(dateWidget);
        dateLayout->setContentsMargins(0, 10, 0, 10);
        dateLayout->setSpacing(2);

        QLabel* dayLabel = new QLabel(scan->getCreatedOn().toString("dd"));
        dayLabel->setStyleSheet(
            "color: #333333; font-size: 22px; font-weight: bold;");

        QLabel* monthLabel =
            new QLabel(scan->getCreatedOn().toString("MMM yyyy"));
        monthLabel->setStyleSheet("color: #666666; font-size: 14px;");

        dateLayout->addWidget(dayLabel, 0, Qt::AlignLeft);
        dateLayout->addWidget(monthLabel, 0, Qt::AlignLeft);
        dateWidget->setFixedWidth(100);

        QWidget* vitalsWidget = new QWidget;
        vitalsWidget->setStyleSheet("border: none;");
        QVBoxLayout* vitalsLayout = new QVBoxLayout(vitalsWidget);
        vitalsLayout->setContentsMargins(0, 10, 0, 10);
        vitalsLayout->setSpacing(2);

        QLabel* tempLabel =
            new QLabel(QString("Temperature: %1°C").arg(scan->getBodyTemp()));
        tempLabel->setStyleSheet("color: #333333; font-size: 15px;");

        QLabel* otherVitalsLabel =
            new QLabel(QString("HR: %1 bpm  •  BP: %2 mmHg")
                           .arg(scan->getHeartRate())
                           .arg(scan->getBloodPressure()));
        otherVitalsLabel->setStyleSheet("color: #666666; font-size: 14px;");

        vitalsLayout->addWidget(tempLabel);
        vitalsLayout->addWidget(otherVitalsLabel);

        cardLayout->addWidget(dateWidget);
        cardLayout->addWidget(vitalsWidget, 1);

        card->setCursor(Qt::PointingHandCursor);
        card->installEventFilter(this);
        card->setProperty("scan_ptr", QVariant::fromValue((void*)scan));
        card->setStyleSheet(
            "#scanCard { "
            "    background-color: white; "
            "    border-radius: 10px; "
            "    border: 1px solid #E0E0E0; "
            "} "
            "#scanCard:hover { "
            "    background-color: #F8F8F8; "
            "    border: 1px solid #D0D0D0; "
            "} "
            "#scanCard > QWidget { "
            "    border: none; "
            "    background: transparent; "
            "} "
            "#scanCard QLabel { "
            "    border: none; "
            "    background: transparent; "
            "}");

        scansGrid->addWidget(card);
    }

    scansGrid->addStretch();
}

/**
 * @brief
 *
 * @param scan
 */
void HistoryWidget::showResultsView(ScanModel* scan) {
    DEBUG("Showing results view");

    scrollArea->hide();
    resultsView->setShowBackButton(true);
    resultsView->setScanModel(*scan);

    connect(resultsView, &ResultsWidget::backButtonClicked, this,
            &HistoryWidget::onBackToHistoryClicked);

    resultsView->show();
}

/**
 * @brief
 */
void HistoryWidget::onBackToHistoryClicked() {
    DEBUG("Back to history clicked");
    showHistoryView();
}

/**
 * @brief
 *
 * @param obj
 * @param event
 * @return
 */
bool HistoryWidget::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QWidget* card = qobject_cast<QWidget*>(obj);
        if (card) {
            QVariant scanPtr = card->property("scan_ptr");
            if (scanPtr.isValid()) {
                ScanModel* scan =
                    static_cast<ScanModel*>(scanPtr.value<void*>());
                onScanSelected(scan);
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

/**
 * @brief
 *
 * @param scan
 */
void HistoryWidget::onNewScanStored(const ScanModel& scan) {
    if (scan.getProfileId() == currentProfileId) {
        loadScansForProfile();
        showHistoryView();
    }
}
