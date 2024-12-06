/**
 * @file MeasureNowWidget.cpp
 * @brief Implementation of the MeasureNowWidget class.
 */

#include "MeasureNowWidget.h"

#include <QPainter>
#include <QtWidgets>

#include "BackgroundDelegate.h"
#include "DeviceController.h"
#include "Logging.h"
#include "ProfileModel.h"
#include "ResultsWidget.h"
#include "ScanController.h"
#include "UserProfileController.h"

/**
 * @brief Constructs and initializes the MeasureNowWidget
 * @param parent The parent widget
 */
MeasureNowWidget::MeasureNowWidget(QWidget* parent,
                                   DeviceController* deviceController,
                                   UserProfileController* userProfileController,
                                   ScanController* scanController, int userId)
    : QWidget(parent),
      profileController(userProfileController),
      scanController(scanController),
      currentUserId(userId),
      deviceController(deviceController),
      countdownTimer(nullptr),
      remainingTime(0),
      currentScanPage(1),
      measurementDone(false),
      scanInProgress(false) {
    INFO("Initializing MeasureNowWidget");

    try {
        auto* mainLayout = new QVBoxLayout(this);
        if (!mainLayout) {
            throw std::runtime_error("Failed to create main layout");
        }

        stackedWidget = new QStackedWidget(this);
        if (!stackedWidget) {
            throw std::runtime_error("Failed to create stacked widget");
        }
        stackedWidget->setStyleSheet(
            "QStackedWidget { background-color: white; }");
        mainLayout->addWidget(stackedWidget);

        // Setup UI components and layouts
        initializeUIComponents(mainLayout);

        // Initialize and configure countdown timer
        countdownTimer = new QTimer(this);
        if (!countdownTimer) {
            throw std::runtime_error("Failed to create countdown timer");
        }
        countdownTimer->setInterval(1000);
        connect(countdownTimer, &QTimer::timeout, this,
                &MeasureNowWidget::updateCountdown);

        // Connect to the device controller
        if (this->deviceController) {
            connect(this->deviceController, &DeviceController::dataReceived,
                    this, &MeasureNowWidget::receiveData);
            DEBUG("DeviceController connected successfully");
        } else {
            ERROR("DeviceController is null in setDeviceController");
        }

        // Initialize and configure connection check timer
        connectionTimer = new QTimer(this);
        if (!connectionTimer) {
            throw std::runtime_error("Failed to create connection check timer");
        }
        connectionTimer->setInterval(1000);
        connect(connectionTimer, &QTimer::timeout, this, [this]() {
            if (!this->deviceController->isConnected() &&
                stackedWidget->currentIndex() != 0) {
                DEBUG("Device disconnected - resetting measurement");
                showAlert("Device disconnected - measurement cancelled");
                resetMeasurement();
            }
        });

        // Setup measurement data and pages
        initImagePaths();
        setupPages();

        // Populate profile list if controllers and userId are available
        if (profileComboBox && this->profileController && currentUserId != -1) {
            populateProfileList(profileComboBox);
        }

        DEBUG("MeasureNowWidget initialization completed successfully");
    } catch (const std::exception& e) {
        ERROR("Failed to initialize MeasureNowWidget:" << e.what());
    }
}

/**
 * @brief Creates the results page placeholder
 */
void MeasureNowWidget::createResultsPage() {
    DEBUG("Creating results page");

    resultsWidget = new ResultsWidget(this);

    stackedWidget->addWidget(resultsWidget);
    DEBUG("Results page created successfully");
}

/**
 * @brief Displays the measurement results in the results page
 */
void MeasureNowWidget::displayResults() {
    DEBUG("Displaying results");
    DEBUG("Number of calculated results: " << calculatedResults.size());

    updateButtonState();

    if (!resultsWidget) {
        ERROR("ResultsWidget is not initialized");
        return;
    }

    stackedWidget->setCurrentWidget(resultsWidget);
}

/**
 * @brief Initializes all UI components and their layouts
 * @param mainLayout The main layout to add components to
 */
void MeasureNowWidget::initializeUIComponents(QVBoxLayout* mainLayout) {
    if (!mainLayout) {
        ERROR("Invalid layout pointer provided to initializeUIComponents");
        return;
    }

    DEBUG("Beginning UI components initialization");

    try {
        QWidget* bottomContainer = new QWidget;
        QVBoxLayout* bottomLayout = new QVBoxLayout(bottomContainer);
        bottomLayout->setContentsMargins(0, 0, 0, 10);
        bottomLayout->setSpacing(5);

        auto* alertContainer = new QWidget;
        alertContainer->setFixedHeight(30);
        auto* alertLayout = new QHBoxLayout(alertContainer);
        alertLayout->setContentsMargins(0, 0, 0, 0);

        alertLabel = new QLabel("");
        alertLabel->setStyleSheet(
            "QLabel { "
            "    font-size: 11px; "
            "    color: white; "
            "    background-color: #FF4444; "
            "    border-radius: 10px; "
            "    padding: 5px 20px; "
            "    margin: 0px; "
            "}");
        alertLabel->setAlignment(Qt::AlignCenter);
        alertLabel->setWordWrap(false);
        alertLabel->setFixedHeight(25);
        alertLabel->setVisible(false);

        alertLayout->addWidget(alertLabel, 0, Qt::AlignCenter);

        startStopButton = new QPushButton("Start Measurement");
        startStopButton->setStyleSheet(BUTTON_STYLE);
        startStopButton->setFixedSize(200, 35);

        if (!connect(startStopButton, &QPushButton::clicked, this,
                     &MeasureNowWidget::onStartStopButtonClicked)) {
            WARNING("Failed to connect start/stop button signal");
        }

        bottomLayout->addWidget(alertContainer, 0, Qt::AlignCenter);
        bottomLayout->addWidget(startStopButton, 0, Qt::AlignCenter);

        mainLayout->addStretch(1);
        mainLayout->addWidget(bottomContainer);

        DEBUG("UI components initialized successfully");
    } catch (const std::exception& e) {
        ERROR("Failed to initialize UI components:" << e.what());
    }
}

/**
 * @brief Initializes the image paths for measurement points
 */
void MeasureNowWidget::initImagePaths() {
    DEBUG("Initializing image paths");

    // Define paths for all measurement point images
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

    // Verify all image paths exist
    for (const auto& path : imagePaths) {
        if (!QFile::exists(path)) {
            WARNING("Image file not found: " << path);
        }
    }

    INFO("Initialized " << imagePaths.size() << " image paths");
}

/**
 * @brief Sets up all pages in the measurement workflow
 */
void MeasureNowWidget::setupPages() {
    DEBUG("Setting up pages");

    try {
        createIntroPage();

        for (int i = 1; i <= TOTAL_SCAN_PAGES; ++i) {
            createScanPage(i);
            DEBUG("Created scan page " << i);
        }

        createPostScanInputPage();
        createResultsPage();
        INFO("Successfully set up all pages");

    } catch (const std::exception& e) {
        ERROR("Failed to setup pages: " << e.what());
    }
}

/**
 * @brief Creates the introduction page with setup instructions
 */
void MeasureNowWidget::createIntroPage() {
    DEBUG("Creating intro page");

    auto* introPage = new QWidget;
    auto* layout = new QVBoxLayout(introPage);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(30);

    auto* contentContainer = new QWidget;
    contentContainer->setObjectName("contentContainer");
    contentContainer->setStyleSheet(
        "#contentContainer {"
        "    background-color: white;"
        "    border-radius: 10px;"
        "    border: 1px solid #E0E0E0;"
        "}");

    auto* containerLayout = new QVBoxLayout(contentContainer);
    containerLayout->setSpacing(30);
    containerLayout->setContentsMargins(40, 40, 40, 40);

    auto* headerLabel = new QLabel("New Measurement");
    headerLabel->setStyleSheet(
        "font-size: 32px;"
        "font-weight: bold;"
        "color: #333333;");
    headerLabel->setAlignment(Qt::AlignCenter);

    auto* introContainer = new QWidget;
    introContainer->setStyleSheet(
        "background-color: #F8F9FA;"
        "border-radius: 15px;"
        "padding: 30px;");

    auto* introLayout = new QVBoxLayout(introContainer);
    introLayout->setSpacing(20);

    QVector<QString> instructions = {
        "Ensure your device is powered on and connected",
        "Hold the device steady at each measurement point",
        "Wait for the measurement to complete",
        "Complete all 24 measurement points in sequence",
        "Follow the on-screen instructions carefully"};

    for (const auto& instruction : instructions) {
        auto* stepLabel = new QLabel("• " + instruction);
        stepLabel->setStyleSheet(
            "font-size: 16px;"
            "color: #555555;"
            "padding: 0px 0px;");
        introLayout->addWidget(stepLabel);
    }

    auto* profileContainer = new QWidget;
    auto* profileLayout = new QVBoxLayout(profileContainer);
    profileLayout->setSpacing(15);

    auto* profileLabel = new QLabel("Select Profile:");
    profileLabel->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #333333;");

    profileComboBox = new QComboBox();
    profileComboBox->setStyleSheet(COMBOBOX_STYLE);
    profileComboBox->setItemDelegate(new BackgroundDelegate(profileComboBox));
    profileComboBox->setMinimumHeight(45);

    connect(
        profileComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [this](int index) {
            selectedProfileId = profileComboBox->itemData(index).toInt();
            emit profileSelected(selectedProfileId, "Name");
            DEBUG(QString("Selected profile ID: %1").arg(selectedProfileId));
        });

    populateProfileList(profileComboBox);

    profileLayout->addWidget(profileLabel);
    profileLayout->addWidget(profileComboBox);

    containerLayout->addWidget(headerLabel);
    containerLayout->addWidget(introContainer);
    containerLayout->addWidget(profileContainer);
    containerLayout->addStretch();

    auto* noteLabel =
        new QLabel("Make sure to hold the device until scan is complete");
    noteLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #666666;"
        "font-style: italic;");
    noteLabel->setAlignment(Qt::AlignCenter);
    containerLayout->addWidget(noteLabel);

    layout->addWidget(contentContainer);
    stackedWidget->addWidget(introPage);

    DEBUG("Intro page created successfully");
}

/**
 * @brief Creates a measurement scan page for the specified point
 * @param pageNum The page number (1-24) determining scan point and body side
 */
void MeasureNowWidget::createScanPage(int pageNum) {
    DEBUG("Creating scan page " << pageNum);

    auto* scanPage = new QWidget;
    scanPage->setStyleSheet("background-color: white;");
    auto* layout = new QVBoxLayout(scanPage);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(30);

    auto* contentContainer = new QWidget;
    contentContainer->setObjectName("contentContainer");
    contentContainer->setStyleSheet(
        "#contentContainer {"
        "    background-color: white;"
        "    border-radius: 10px;"
        "    border: 1px solid #E0E0E0;"
        "}");

    auto* containerLayout = new QVBoxLayout(contentContainer);
    containerLayout->setSpacing(25);
    containerLayout->setContentsMargins(30, 30, 30, 30);

    bool isRightSide = pageNum > MEASUREMENTS_PER_SIDE;
    int imageIndex =
        isRightSide ? (pageNum - MEASUREMENTS_PER_SIDE - 1) : (pageNum - 1);
    QString sideText = isRightSide ? "Right" : "Left";

    auto* headerContainer = new QWidget;
    headerContainer->setFixedHeight(40);
    auto* headerLayout = new QHBoxLayout(headerContainer);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    QString progressText =
        QString("Point %1 of %2")
            .arg(isRightSide ? pageNum - MEASUREMENTS_PER_SIDE : pageNum)
            .arg(MEASUREMENTS_PER_SIDE);
    auto* progressLabel = new QLabel(progressText);
    progressLabel->setStyleSheet(
        "font-size: 16px;"
        "color: #666666;");

    auto* sideLabel = new QLabel(sideText + " Side");
    sideLabel->setStyleSheet(
        "font-size: 16px;"
        "font-weight: bold;"
        "color: #333333;");

    headerLayout->addWidget(progressLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(sideLabel);

    QWidget* imageContainer = new QWidget;
    QVBoxLayout* imageLayout = new QVBoxLayout(imageContainer);
    imageLayout->setContentsMargins(0, 0, 0, 0);
    imageLayout->setAlignment(Qt::AlignCenter);

    auto* imageLabel = new QLabel();
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageLabel->setObjectName("scanImageLabel");
    imageLabel->setMinimumSize(200, 200);

    if (imageIndex >= 0 && imageIndex < imagePaths.size()) {
        QPixmap originalPixmap(imagePaths.at(imageIndex));
        if (!originalPixmap.isNull()) {
            if (isRightSide) {
                originalPixmap =
                    originalPixmap.transformed(QTransform().scale(-1, 1));
            }
            originalPixmaps[pageNum] = originalPixmap;

            QSize initialSize(400, 400);
            imageLabel->setPixmap(originalPixmap.scaled(
                initialSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }

    imageLayout->addWidget(imageLabel, 0, Qt::AlignCenter);

    auto* statusContainer = new QWidget;
    statusContainer->setFixedHeight(100);
    statusContainer->setStyleSheet(
        "QWidget {"
        "    background-color: #F8F9FA;"
        "    border-radius: 10px;"
        "}");

    auto* statusLayout = new QVBoxLayout(statusContainer);
    statusLayout->setContentsMargins(20, 15, 20, 15);

    auto* instructionLabel = new QLabel("Place device on highlighted point");
    instructionLabel->setStyleSheet(
        "font-size: 15px;"
        "color: #666666;");
    instructionLabel->setAlignment(Qt::AlignCenter);

    auto* statusLabel = new QLabel("Waiting for measurement...");
    statusLabel->setStyleSheet(
        "font-size: 16px;"
        "font-weight: bold;"
        "color: #333333;");
    statusLabel->setAlignment(Qt::AlignCenter);
    countdownLabels.append(statusLabel);

    statusLayout->addWidget(instructionLabel);
    statusLayout->addWidget(statusLabel);

    containerLayout->addWidget(headerContainer);
    containerLayout->addWidget(imageContainer, 1);
    containerLayout->addWidget(statusContainer);

    layout->addWidget(contentContainer);
    stackedWidget->addWidget(scanPage);

    QTimer::singleShot(0, [this, pageNum]() { adjustImageSize(pageNum); });

    DEBUG("Scan page " << pageNum << " created successfully");
}

/**
 * @brief Initiates the measurement countdown process
 */
void MeasureNowWidget::startCountdown() {
    int currentIndex = stackedWidget->currentIndex();
    bool isOnScanPage =
        currentIndex > 0 && currentIndex < stackedWidget->count() - 1;

    if (!isOnScanPage) {
        DEBUG("Cannot start measurement - not on a scan page");
        return;
    }

    if (!deviceController || !deviceController->isDeviceOn() ||
        !deviceController->isConnected()) {
        DEBUG("Cannot start measurement - device not connected");
        return;
    }

    if (scanInProgress) {
        DEBUG("Measurement already in progress");
        return;
    }

    DEBUG("Starting new measurement");
    resetState();

    int scanPageIndex = currentIndex - 1;
    if (scanPageIndex >= 0 && scanPageIndex < countdownLabels.size()) {
        countdownLabels[scanPageIndex]->setText("Measuring...");
    }

    measurementDone = false;
    scanInProgress = true;
    countdownTimer->start();
}

/**
 * @brief Handles completion of a measurement
 */
void MeasureNowWidget::measurementComplete() {
    DEBUG("Measurement complete");

    if (deviceController) {
        deviceController->transmitData();
    } else {
        ERROR("DeviceController is null in measurementComplete");
        return;
    }

    measurementDone = true;
}

/**
 * @brief Processes received measurement data
 * @param data The measurement value received from the device
 */
void MeasureNowWidget::receiveData(int data) {
    DEBUG("Received data: " << data);

    int currentPage = stackedWidget->currentIndex();
    DEBUG("Current page: " << currentPage);

    ScanPoint scanPoint{currentPage, data};
    rawMeasurements.append(scanPoint);
    DEBUG("Total measurements collected: " << rawMeasurements.size());

    showMeasurementResult(data);
}

/**
 * @brief Processes all collected measurements and calculates final results
 */
void MeasureNowWidget::processMeasurements() {
    if (selectedProfileId == -1) {
        WARNING("No profile selected");
        return;
    }

    DEBUG("Processing measurements");

    if (rawMeasurements.size() != TOTAL_SCAN_PAGES) {
        ERROR("Not all measurements have been collected");
        return;
    }

    ScanModel scanModel;
    scanModel.setProfileId(selectedProfileId);

    QVector<int> measurements(TOTAL_SCAN_PAGES);  // 24 measurements

    std::sort(rawMeasurements.begin(), rawMeasurements.end(),
              [](const ScanPoint& a, const ScanPoint& b) {
                  return a.position < b.position;
              });

    for (int i = 0; i < rawMeasurements.size(); ++i) {
        measurements[i] = rawMeasurements[i].rawValue;
    }

    scanModel.setName("Name");

    // Left Side Measurements (positions 1-12)
    scanModel.setH1Lung(measurements[0]);              // Lungs (Left)
    scanModel.setH2HeartConstrictor(measurements[1]);  // Pericardium (Left)
    scanModel.setH3Heart(measurements[2]);             // Heart (Left)
    scanModel.setH4SmallIntestine(measurements[3]);    // Small Intestine (Left)
    scanModel.setH5TripleHeater(measurements[4]);      // Immune System (Left)
    scanModel.setH6LargeIntestine(measurements[5]);    // Large Intestine (Left)
    scanModel.setF1Spleen(measurements[6]);  // Pancreas and Spleen (Left)
    scanModel.setF2Liver(measurements[7]);   // Liver (Left)
    scanModel.setF3Kidney(measurements[8]);  // Kidney (Left)
    scanModel.setF4UrinaryBladder(measurements[9]);  // Bladder (Left)
    scanModel.setF5GallBladder(measurements[10]);    // Gallbladder (Left)
    scanModel.setF6Stomach(measurements[11]);        // Stomach (Left)

    // Right Side Measurements (positions 13-24)
    scanModel.setH1LungR(measurements[12]);              // Lungs (Right)
    scanModel.setH2HeartConstrictorR(measurements[13]);  // Pericardium (Right)
    scanModel.setH3HeartR(measurements[14]);             // Heart (Right)
    scanModel.setH4SmallIntestineR(
        measurements[15]);                           // Small Intestine (Right)
    scanModel.setH5TripleHeaterR(measurements[16]);  // Immune System (Right)
    scanModel.setH6LargeIntestineR(
        measurements[17]);                     // Large Intestine (Right)
    scanModel.setF1SpleenR(measurements[18]);  // Pancreas and Spleen (Right)
    scanModel.setF2LiverR(measurements[19]);   // Liver (Right)
    scanModel.setF3KidneyR(measurements[20]);  // Kidney (Right)
    scanModel.setF4UrinaryBladderR(measurements[21]);  // Bladder (Right)
    scanModel.setF5GallBladderR(measurements[22]);     // Gallbladder (Right)
    scanModel.setF6StomachR(measurements[23]);         // Stomach (Right)

    scanModel.setMeasurements(measurements);

    scanModel.setBodyTemp(bodyTemp);
    scanModel.setBloodPressure(bloodPressure);
    scanModel.setHeartRate(heartRate);
    scanModel.setSleepingTime(sleepingTime);
    scanModel.setCurrentWeight(currentWeight);
    scanModel.setEmotionalState(emotionalState);
    scanModel.setOverallFeeling(overallFeeling);

    if (!scanController->storeScan(scanModel)) {
        ERROR("Failed to store the scan");
        return;
    }

    if (!resultsWidget) {
        resultsWidget = new ResultsWidget(this);
        stackedWidget->addWidget(resultsWidget);
    }
    resultsWidget->setShowBackButton(false);
    resultsWidget->setScanModel(scanModel);

    displayResults();
}

/**
 * @brief Checks if all measurements are complete
 * @return True if all measurements have been collected
 */
bool MeasureNowWidget::areAllMeasurementsComplete() const {
    return rawMeasurements.size() == TOTAL_SCAN_PAGES;
}

/**
 * @brief Updates the UI with the measurement result
 * @param value The measurement value to display
 */
void MeasureNowWidget::showMeasurementResult(int value) {
    int currentIndex = stackedWidget->currentIndex() - 1;
    if (currentIndex >= 0 && currentIndex < countdownLabels.size()) {
        auto* statusLabel = countdownLabels[currentIndex];
        statusLabel->setText(QString("Measurement value: %1").arg(value));
    }
}

/**
 * @brief Handles the start/stop button click event
 */
void MeasureNowWidget::onStartStopButtonClicked() {
    int currentIndex = stackedWidget->currentIndex();
    DEBUG("Start/Stop button clicked. Current page index: " << currentIndex);

    if (currentIndex == 0) {
        if (!deviceController || !deviceController->isDeviceOn() ||
            !deviceController->isConnected()) {
            showAlert("Device is off or not connected");
            return;
        }

        if (connectionTimer) {
            connectionTimer->start();
        }
        nextPage();

    } else {
        if (connectionTimer) {
            connectionTimer->stop();
        }
        resetMeasurement();
    }
}

/**
 * @brief Handles the image release event during measurement
 */
void MeasureNowWidget::onImageReleased() {
    if (!deviceController || !deviceController->isDeviceOn() ||
        !deviceController->isConnected()) {
        return;
    }

    int currentIndex = stackedWidget->currentIndex();
    bool isOnScanPage =
        currentIndex > 0 && currentIndex < stackedWidget->count() - 1;

    if (measurementDone) {
        DEBUG("Measurement done, proceeding to next page");
        nextPage();
        measurementDone = false;
        scanInProgress = false;
    } else if (scanInProgress && isOnScanPage) {
        DEBUG("Measurement interrupted");
        handleScanError();
    }
    scanInProgress = false;
}

/**
 * @brief Advances to the next page in the measurement workflow
 */
void MeasureNowWidget::nextPage() {
    int nextIndex = stackedWidget->currentIndex() + 1;
    if (nextIndex < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(nextIndex);
        currentScanPage++;
        updateButtonState();

        if (nextIndex > 0 && nextIndex < stackedWidget->count() - 1) {
            adjustImageSize(nextIndex);
        }

        if (nextIndex == stackedWidget->count() - 2) {
            DEBUG("Reached post-scan input page");
        } else if (nextIndex == stackedWidget->count() - 1) {
            DEBUG("About to show results page, processing measurements");
            processMeasurements();
        }
    }
}

/**
 * @brief
 */
void MeasureNowWidget::createPostScanInputPage() {
    DEBUG("Creating post-scan input page");

    QWidget* inputPage = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(inputPage);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(30);

    QWidget* contentContainer = new QWidget;
    contentContainer->setObjectName("contentContainer");
    contentContainer->setStyleSheet(
        "#contentContainer {"
        "    background-color: white;"
        "    border-radius: 10px;"
        "    border: 1px solid #E0E0E0;"
        "}");

    QVBoxLayout* containerLayout = new QVBoxLayout(contentContainer);
    containerLayout->setSpacing(25);
    containerLayout->setContentsMargins(30, 30, 30, 30);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->setSpacing(15);

    bodyTempEdit = new QDoubleSpinBox;
    bodyTempEdit->setRange(30.0, 40.0);
    bodyTempEdit->setDecimals(1);
    bodyTempEdit->setSuffix(" °C");
    bodyTempEdit->setValue(36.5);
    bodyTempEdit->setStyleSheet(INPUT_STYLE);

    bloodPressureEdit = new QSpinBox;
    bloodPressureEdit->setRange(80, 180);
    bloodPressureEdit->setSuffix(" mmHg");
    bloodPressureEdit->setValue(120);
    bloodPressureEdit->setStyleSheet(INPUT_STYLE);

    heartRateEdit = new QSpinBox;
    heartRateEdit->setRange(40, 180);
    heartRateEdit->setSuffix(" bpm");
    heartRateEdit->setValue(70);
    heartRateEdit->setStyleSheet(INPUT_STYLE);

    sleepingTimeEdit = new QDoubleSpinBox;
    sleepingTimeEdit->setRange(2.0, 12.0);
    sleepingTimeEdit->setDecimals(1);
    sleepingTimeEdit->setSuffix(" hours");
    sleepingTimeEdit->setValue(7.0);
    sleepingTimeEdit->setStyleSheet(INPUT_STYLE);

    currentWeightEdit = new QDoubleSpinBox;
    currentWeightEdit->setRange(20.0, 300.0);
    currentWeightEdit->setDecimals(1);
    currentWeightEdit->setSuffix(" kg");
    currentWeightEdit->setValue(70.0);
    currentWeightEdit->setStyleSheet(INPUT_STYLE);

    emotionalStateEdit = new QComboBox;
    emotionalStateEdit->addItems({"1", "2", "3", "4", "5"});
    emotionalStateEdit->setCurrentIndex(2);
    emotionalStateEdit->setStyleSheet(COMBOBOX_STYLE);
    emotionalStateEdit->setItemDelegate(
        new BackgroundDelegate(emotionalStateEdit));

    overallFeelingEdit = new QComboBox;
    overallFeelingEdit->addItems({"1", "2", "3", "4", "5"});
    overallFeelingEdit->setCurrentIndex(2);
    overallFeelingEdit->setStyleSheet(COMBOBOX_STYLE);
    overallFeelingEdit->setItemDelegate(
        new BackgroundDelegate(overallFeelingEdit));

    QString labelStyle = "font-weight: bold; color: #333333; font-size: 14px;";
    auto addFormRow = [&](const QString& labelText, QWidget* widget) {
        QLabel* label = new QLabel(labelText);
        label->setStyleSheet(labelStyle);
        formLayout->addRow(label, widget);
    };

    addFormRow("Body Temperature:", bodyTempEdit);
    addFormRow("Blood Pressure:", bloodPressureEdit);
    addFormRow("Heart Rate:", heartRateEdit);
    addFormRow("Sleeping Time:", sleepingTimeEdit);
    addFormRow("Current Weight:", currentWeightEdit);
    addFormRow("Emotional State (1-5):", emotionalStateEdit);
    addFormRow("Overall Feeling (1-5):", overallFeelingEdit);

    QLabel* instructionsLabel =
        new QLabel("Please enter the following information:");
    instructionsLabel->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #333333;");
    instructionsLabel->setAlignment(Qt::AlignCenter);

    QPushButton* proceedButton = new QPushButton("Proceed to Results");
    proceedButton->setStyleSheet(BUTTON_STYLE);
    proceedButton->setMinimumHeight(40);
    proceedButton->setFixedWidth(200);

    connect(proceedButton, &QPushButton::clicked, this, [this]() {
        collectUserInputs();
        nextPage();
    });

    containerLayout->addWidget(instructionsLabel);
    containerLayout->addLayout(formLayout);
    containerLayout->addStretch();
    containerLayout->addWidget(proceedButton, 0, Qt::AlignCenter);

    layout->addWidget(contentContainer);
    stackedWidget->addWidget(inputPage);
    DEBUG("Post-scan input page created successfully");
}

/**
 * @brief
 */
void MeasureNowWidget::collectUserInputs() {
    bodyTemp = bodyTempEdit->value();
    bloodPressure = bloodPressureEdit->value();
    heartRate = heartRateEdit->value();
    sleepingTime = sleepingTimeEdit->value();
    currentWeight = currentWeightEdit->value();
    emotionalState = emotionalStateEdit->currentText().toInt();
    overallFeeling = overallFeelingEdit->currentText().toInt();

    DEBUG(QString("Collected user inputs: Body Temp=%1°C, Blood Pressure=%2 "
                  "mmHg, Heart Rate=%3 bpm, Sleeping Time=%4 hours, Current "
                  "Weight=%5 kg, Emotional State=%6, Overall Feeling=%7")
              .arg(bodyTemp)
              .arg(bloodPressure)
              .arg(heartRate)
              .arg(sleepingTime)
              .arg(currentWeight)
              .arg(emotionalState)
              .arg(overallFeeling));
}

/**
 * @brief Resets the measurement process to initial state
 */
void MeasureNowWidget::resetMeasurement() {
    DEBUG("Resetting measurement");

    if (connectionTimer) {
        connectionTimer->stop();
    }

    resetState();
    currentScanPage = 1;
    measurementDone = false;
    rawMeasurements.clear();
    stackedWidget->setCurrentIndex(0);
    updateButtonState();
}

/**
 * @brief Resets the current measurement state
 */
void MeasureNowWidget::resetState() {
    bool wasScanning = scanInProgress;
    countdownTimer->stop();
    scanInProgress = false;

    int currentIndex = stackedWidget->currentIndex() - 1;
    if (currentIndex >= 0 && currentIndex < countdownLabels.size()) {
        countdownLabels[currentIndex]->setText(
            "Place device on measurement point");
    }

    if (wasScanning) {
        DEBUG("Reset state: Cancelled active scan");
    } else {
        DEBUG("Reset state: Cleared previous state");
    }
}

/**
 * @brief Updates the start/stop button text based on current page
 */
void MeasureNowWidget::updateButtonState() {
    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex == 0) {
        startStopButton->setText("Start Measurement");
    } else if (currentIndex == stackedWidget->count() - 1) {
        startStopButton->setText("Finish");
    } else {
        startStopButton->setText("Stop");
    }
}

/**
 * @brief Updates the countdown display during measurement
 */
void MeasureNowWidget::updateCountdown() {
    if (!deviceController || !deviceController->isConnected()) {
        DEBUG("Device disconnected during measurement");
        showAlert("Device disconnected - measurement cancelled");
        resetMeasurement();
        return;
    }

    int currentIndex = stackedWidget->currentIndex() - 1;
    if (currentIndex >= 0 && currentIndex < countdownLabels.size()) {
        if (remainingTime > 0) {
            remainingTime--;
        } else {
            countdownTimer->stop();
            measurementComplete();
        }
    }
}

/**
 * @brief Populates the profile selection combo box
 * @param profileComboBox The combo box to populate
 */
void MeasureNowWidget::populateProfileList(QComboBox* comboBox) {
    if (!comboBox || !profileController) {
        ERROR("Invalid combo box or profile controller not set");
        return;
    }

    DEBUG("Populating profile list");
    comboBox->clear();

    QVector<ProfileModel*> profiles;
    if (profileController->getProfiles(currentUserId, profiles)) {
        for (const ProfileModel* profile : profiles) {
            if (profile) {
                comboBox->addItem(profile->getName(), profile->getId());
            }
        }
        DEBUG(QString("Added %1 profiles to combo box").arg(profiles.size()));
    } else {
        WARNING("Failed to load profiles");
    }

    qDeleteAll(profiles);
    profiles.clear();
}

/**
 * @brief Sets the error state styling for a label
 * @param label The label to style
 */
void MeasureNowWidget::setErrorState(QLabel* label) {
    if (!label) return;

    label->setText("Image not found");
    label->setStyleSheet(
        "QLabel { "
        "    font-size: 14px; "
        "    color: #999999; "
        "    border: 2px dashed #cccccc; "
        "    padding: 20px; "
        "    background-color: #f5f5f5; "
        "}");
}

/**
 * @brief Handles errors during the scanning process
 */
void MeasureNowWidget::handleScanError() {
    DEBUG("Handling scan error");

    countdownTimer->stop();

    int currentIndex = stackedWidget->currentIndex() - 1;
    if (currentIndex >= 0 && currentIndex < countdownLabels.size()) {
        countdownLabels[currentIndex]->setText(
            "Place device on measurement point");
    }

    showAlert("Scan interrupted - Please hold device until scan is complete");
}

/**
 * @brief Shows an alert message to the user
 * @param message The message to display
 * @param autoHide Whether to automatically hide the message after a delay
 */
void MeasureNowWidget::showAlert(const QString& message, bool autoHide) {
    DEBUG("Showing alert: " << message);

    alertLabel->setText(message);
    alertLabel->setVisible(true);

    if (autoHide) {
        QTimer::singleShot(3000, [this]() {
            alertLabel->clear();
            alertLabel->setVisible(false);
        });
    }
}

/**
 * @brief Refresh the profiles
 */
void MeasureNowWidget::refreshProfiles() {
    if (profileComboBox && profileController) {
        DEBUG("Refreshing profile list");
        populateProfileList(profileComboBox);
    }
}

/**
 * @brief Update the user id
 */
void MeasureNowWidget::setUserId(int userId) {
    DEBUG(QString("Setting user ID: %1").arg(userId));
    currentUserId = userId;
    if (profileComboBox && profileController) {
        populateProfileList(profileComboBox);
    }
}

/**
 * @brief
 *
 * @param pageIndex
 */
void MeasureNowWidget::adjustImageSize(int pageIndex) {
    QWidget* page = stackedWidget->widget(pageIndex);
    if (!page) return;

    QLabel* imageLabel = page->findChild<QLabel*>("scanImageLabel");
    if (!imageLabel) return;

    if (originalPixmaps.contains(pageIndex)) {
        QPixmap originalPixmap = originalPixmaps[pageIndex];

        QWidget* container = imageLabel->parentWidget();
        if (!container) return;

        QSize containerSize = container->size();

        int dimension = qMin(containerSize.width(), containerSize.height());
        dimension = qMin(dimension, 600);
        dimension = qMax(dimension, 200);

        QSize targetSize(dimension, dimension);

        if (!originalPixmap.isNull()) {
            imageLabel->setPixmap(originalPixmap.scaled(
                targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

/**
 * @brief
 *
 * @param event
 */
void MeasureNowWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex > 0 && currentIndex < stackedWidget->count() - 1) {
        adjustImageSize(currentIndex);
    }
}
