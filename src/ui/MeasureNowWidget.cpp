/**
 * @file MeasureNowWidget.cpp
 * @brief Implementation of the MeasureNowWidget class.
 */

#include "MeasureNowWidget.h"

#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "DeviceController.h"

/* Logging Macros */
#define DEBUG(msg) qDebug() << "[DEBUG]" << __FUNCTION__ << ":" << msg
#define INFO(msg) qInfo() << "[INFO]" << __FUNCTION__ << ":" << msg
#define ERROR(msg) qCritical() << "[ERROR]" << __FUNCTION__ << ":" << msg

MeasureNowWidget::MeasureNowWidget(QWidget *parent)
    : QWidget(parent), currentScanPage(1), measurementDone(false) {
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create stacked widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("QWidget { background-color: white; }");
    mainLayout->addWidget(stackedWidget);

    // Create the start/stop button
    startStopButton = new QPushButton("Start");
    QString buttonStyle =
        "QPushButton {"
        "    background-color: #FF7009;"
        "    color: white;"
        "    border-radius: 20px;"
        "    padding: 5px 20px;"
        "    font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #E66008;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #CC5A07;"
        "}";
    startStopButton->setStyleSheet(buttonStyle);
    startStopButton->setMinimumHeight(40);
    startStopButton->setFixedWidth(200);

    connect(startStopButton, &QPushButton::clicked, this,
            &MeasureNowWidget::onStartStopButtonClicked);

    // Add the button to the main layout at the bottom
    mainLayout->addWidget(startStopButton, 0, Qt::AlignCenter);

    // Setup countdown timer
    countdownTimer = new QTimer(this);
    countdownTimer->setInterval(1000);
    connect(countdownTimer, &QTimer::timeout, this,
            &MeasureNowWidget::updateCountdown);

    // Load the images
    initImagePaths();

    // Setup all pages
    setupPages();
}

void MeasureNowWidget::onStartStopButtonClicked() {
    if (stackedWidget->currentIndex() == 0) {
        // Intro page, start measurement
        nextPage();
    } else if (stackedWidget->currentIndex() == stackedWidget->count() - 1) {
        // Results page, finish measurement
        resetMeasurement();
    } else {
        // Scanning pages, stop measurement
        // QMessageBox::StandardButton reply;
        // reply = QMessageBox::question(
        //     this, "Stop Measurement",
        //     "Do you want to stop the measurement and discard progress?",
        //     QMessageBox::Yes | QMessageBox::No);
        // if (reply == QMessageBox::Yes) {
        //     resetMeasurement();
        // }
        resetMeasurement();
    }
}

void MeasureNowWidget::resetMeasurement() {
    countdownTimer->stop();
    resetLabel();
    currentScanPage = 1;
    measurementDone = false;
    measurementData.clear();
    stackedWidget->setCurrentIndex(0);
    updateButtonState();
}

void MeasureNowWidget::updateButtonState() {
    if (stackedWidget->currentIndex() == 0) {
        // Intro page
        startStopButton->setText("Start Measurement");
    } else if (stackedWidget->currentIndex() == stackedWidget->count() - 1) {
        // Results page
        startStopButton->setText("Finish");
    } else {
        // Scanning pages
        startStopButton->setText("Stop");
    }
}

void MeasureNowWidget::createIntroPage() {
    QWidget *introPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(introPage);

    QLabel *introLabel = new QLabel(
        "You will be guided through 12 measurement points.\n"
        "Click and hold the device for 2 seconds at each point.");
    introLabel->setStyleSheet("QLabel { font-size: 16px; color: #333333; }");
    introLabel->setAlignment(Qt::AlignCenter);
    introLabel->setWordWrap(true);

    alertLabel = new QLabel("");
    alertLabel->setStyleSheet("QLabel { font-size: 14px; color: red; }");
    alertLabel->setAlignment(Qt::AlignCenter);

    layout->addStretch();
    layout->addWidget(introLabel);
    layout->addSpacing(20);
    layout->addWidget(alertLabel);
    layout->addStretch();

    stackedWidget->addWidget(introPage);
}

void MeasureNowWidget::createScanPage(int pageNum) {
    QWidget *scanPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(scanPage);

    QLabel *pageLabel = new QLabel(
        QString("Scan Point %1 of %2").arg(pageNum).arg(TOTAL_SCAN_PAGES));
    pageLabel->setStyleSheet(
        "QLabel { font-size: 18px; color: #333333; font-weight: bold; }");
    pageLabel->setAlignment(Qt::AlignCenter);

    // Create countdown label for this page
    QLabel *countdownLabel = new QLabel("Hold for 2 seconds to measure");
    countdownLabel->setStyleSheet(
        "QLabel { font-size: 16px; color: #666666; }");
    countdownLabel->setAlignment(Qt::AlignCenter);
    countdownLabels.append(countdownLabel);  // Store the label

    QLabel *imageLabel = new QLabel();
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setMinimumSize(300, 200);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Ensure pageNum is within the range of imagePaths
    if (pageNum >= 1 && pageNum <= imagePaths.size()) {
        QString imagePath = imagePaths.at(pageNum - 1);  // Zero-based index

        QPixmap pixmap(imagePath);

        if (!pixmap.isNull()) {
            // Scale the pixmap to fit the label while maintaining aspect ratio
            pixmap = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation);
            imageLabel->setPixmap(pixmap);
        } else {
            // Handle the case where the image couldn't be loaded
            imageLabel->setText("Image not found");
            imageLabel->setStyleSheet(
                "QLabel { "
                "    font-size: 14px; "
                "    color: #999999; "
                "    border: 2px dashed #cccccc; "
                "    padding: 20px; "
                "    background-color: #f5f5f5; "
                "}");
        }
    } else {
        // Handle invalid page numbers
        imageLabel->setText("Invalid page number");
        imageLabel->setStyleSheet(
            "QLabel { "
            "    font-size: 14px; "
            "    color: #999999; "
            "    border: 2px dashed #cccccc; "
            "    padding: 20px; "
            "    background-color: #f5f5f5; "
            "}");
    }

    layout->addStretch();
    layout->addWidget(pageLabel);
    layout->addSpacing(10);
    layout->addWidget(countdownLabel);
    layout->addSpacing(20);
    layout->addWidget(imageLabel);
    layout->addStretch();

    stackedWidget->addWidget(scanPage);
}

void MeasureNowWidget::resetLabel() {
    // Get the current page index (subtract 1 for intro page)
    int currentIndex = stackedWidget->currentIndex() - 1;
    if (currentIndex >= 0 && currentIndex < countdownLabels.size()) {
        countdownLabels[currentIndex]->setText("Hold for 2 seconds to measure");
    }
}

void MeasureNowWidget::updateCountdown() {
    int currentIndex = stackedWidget->currentIndex() - 1;
    if (currentIndex >= 0 && currentIndex < countdownLabels.size()) {
        if (remainingTime > 0) {
            countdownLabels[currentIndex]->setText(
                QString("Keep holding... %1").arg(remainingTime));
            remainingTime--;
        } else {
            countdownTimer->stop();
            countdownLabels[currentIndex]->setText("Measurement complete!");

            // Complete measurement after updating label
            measurementComplete();
        }
    }
}

void MeasureNowWidget::createResultsPage() {
    QWidget *resultsPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(resultsPage);

    QLabel *resultsLabel = new QLabel("Measurement Results");
    resultsLabel->setStyleSheet(
        "QLabel { font-size: 20px; color: #333333; font-weight: bold; }");
    resultsLabel->setAlignment(Qt::AlignCenter);

    QLabel *resultsContent = new QLabel("[Placeholder for Results]");
    resultsContent->setStyleSheet(
        "QLabel { "
        "    font-size: 16px; "
        "    color: #666666; "
        "    background-color: #f5f5f5; "
        "    padding: 20px; "
        "    border-radius: 4px; "
        "}");
    resultsContent->setAlignment(Qt::AlignCenter);

    layout->addStretch();
    layout->addWidget(resultsLabel);
    layout->addSpacing(20);
    layout->addWidget(resultsContent);
    layout->addStretch();

    stackedWidget->addWidget(resultsPage);
}

void MeasureNowWidget::setupPages() {
    // Create intro page
    createIntroPage();

    // Create scan pages
    for (int i = 1; i <= TOTAL_SCAN_PAGES; ++i) {
        createScanPage(i);
    }

    // Create results page
    createResultsPage();
}

void MeasureNowWidget::startCountdown() {
    measurementDone = false;  // Reset at the start
    remainingTime = 2;
    updateCountdown();
    countdownTimer->start();
}

void MeasureNowWidget::cancelCountdown() {
    countdownTimer->stop();
    resetLabel();
}

void MeasureNowWidget::measurementComplete() {
    countdownTimer->stop();  // Ensure timer is stopped
    measurementDone = true;  // Mark measurement as done
    // Update label is done in updateCountdown()

    if (deviceController) {
        deviceController->transmitData();
    } else {
        ERROR("DeviceController is null in measurementComplete");
    }
}

void MeasureNowWidget::nextPage() {
    if (stackedWidget->currentIndex() == 0) {
        if (!deviceController || !deviceController->isDeviceOn() ||
            !deviceController->isConnected()) {
            alertLabel->setText("Device is off or not connected");
            QTimer::singleShot(3000, [this]() { alertLabel->setText(""); });
            INFO("Device is off or not connected");
            return;
        }
    }

    int nextIndex = stackedWidget->currentIndex() + 1;
    if (nextIndex < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(nextIndex);
        currentScanPage++;
        updateButtonState();  // Update button text

        if (nextIndex == stackedWidget->count() - 1) {
            // Reached the results page
            displayResults();
        }
    }
}

void MeasureNowWidget::showScanError() {
    // QMessageBox::warning(
    //     this, "Scan Error",
    //     "Please hold the device steady for the full 2 seconds.");
    DEBUG("Please hold the device steady for the full 2 seconds.");
}

void MeasureNowWidget::setDeviceController(DeviceController *controller) {
    deviceController = controller;
    if (deviceController) {
        connect(deviceController, &DeviceController::dataReceived, this,
                &MeasureNowWidget::receiveData);
        // Also, connect any other necessary signals here
        // For example, battery level changes
        // connect(deviceController, &DeviceController::batteryLevelChanged,
        // this, &MeasureNowWidget::onBatteryLevelChanged);
    } else {
        ERROR("DeviceController is null in setDeviceController");
    }
}

void MeasureNowWidget::initImagePaths() {
    imagePaths = QStringList{":/images/lungs_point.png",
                             ":/images/pericardium_point.png",
                             ":/images/heart_point.png",
                             ":/images/small_intestine_point.png",
                             ":/images/immune_system_point.png",
                             ":/images/large_intestine_point.png",
                             ":/images/pancreas_and_spleen_point.png",
                             ":/images/liver_point.png",
                             ":/images/kidney_point.png",
                             ":/images/bladder_point.png",
                             ":/images/gallbladder_point.png",
                             ":/images/stomach_point.png"};
}

void MeasureNowWidget::receiveData(int data) {
    measurementData.append(data);
    DEBUG("Received data: " << data);
}

void MeasureNowWidget::onImageReleased() {
    if (measurementDone) {
        // Measurement completed; proceed to next page
        nextPage();
        measurementDone = false;  // Reset for the next measurement
    } else {
        // Measurement not completed; cancel countdown and show error
        cancelCountdown();
        showScanError();
    }
}

void MeasureNowWidget::displayResults() {
    // For demonstration, display a message box with results
    QStringList results;
    for (int i = 0; i < measurementData.size(); ++i) {
        results.append(
            QString("Point %1: %2").arg(i + 1).arg(measurementData.at(i)));
    }
    QString resultText = results.join('\n');
    // QMessageBox::information(this, "Measurement Results", resultText);
    DEBUG("Measurement Results: " << resultText);

    // Update the button text
    updateButtonState();
}
