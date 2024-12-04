/**
 * @file MeasureNowWidget.cpp
 * @brief Implementation of the MeasureNowWidget class.
 */

#include "MeasureNowWidget.h"

#include <QPainter>
#include <QtWidgets>

#include "DeviceController.h"
#include "Logging.h"
#include "ProfileModel.h"
#include "UserProfileController.h"

namespace {
/**
 * @brief Style definitions for UI components
 *
 * These constants define the CSS-style formatting for various UI elements
 * including buttons, input fields, and combo boxes.
 */

// Button styling with hover and pressed states
const QString BUTTON_STYLE = R"(
        QPushButton {
            background-color: #FF7009;
            color: white;
            border-radius: 20px;
            padding: 5px 20px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #E66008;
        }
        QPushButton:pressed {
            background-color: #CC5A07;
        }
    )";

// Input field styling for date and time editors
const QString INPUT_STYLE =
    "QDateEdit, QTimeEdit {"
    "    background-color: white;"
    "    border: 2px solid #E0E0E0;"
    "    border-radius: 6px;"
    "    padding: 8px;"
    "    font-size: 14px;"
    "    min-width: 100px;"
    "    color: #333333;"
    "}"
    "QDateEdit:hover, QTimeEdit:hover {"
    "    border-color: #FF7009;"
    "}"
    "QDateEdit::up-button, QDateEdit::down-button,"
    "QTimeEdit::up-button, QTimeEdit::down-button {"
    "    width: 0px;"
    "    height: 0px;"
    "    border: none;"
    "    image: none;"
    "}";

// Combo box styling with custom dropdown appearance
const QString COMBOBOX_STYLE =
    "QComboBox {"
    "    background-color: white;"
    "    border: 2px solid #E0E0E0;"
    "    border-radius: 6px;"
    "    padding: 8px;"
    "    font-size: 14px;"
    "    min-width: 200px;"
    "    color: #333333;"
    "}"
    "QComboBox:hover {"
    "    border-color: #FF7009;"
    "}"
    "QComboBox::drop-down {"
    "    border: none;"
    "    background: none;"
    "}"
    "QComboBox::down-arrow {"
    "    image: none;"
    "}"
    "QComboBox QAbstractItemView {"
    "    background-color: white;"
    "    border: 1px solid #E0E0E0;"
    "    selection-background-color: #FF7009;"
    "    selection-color: white;"
    "}";
}  // namespace

/**
 * @brief BackgroundDelegate implementation for custom item styling
 */
BackgroundDelegate::BackgroundDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

/**
 * @brief Custom paint implementation for combo box items
 *
 * Handles the painting of combo box items with custom colors for
 * hover and selection states.
 *
 * @param painter The painter to use for drawing
 * @param option The style options for the item
 * @param index The model index of the item being painted
 */
void BackgroundDelegate::paint(QPainter* painter,
                               const QStyleOptionViewItem& option,
                               const QModelIndex& index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // Set custom colors for hover and selection states
    if (opt.state & QStyle::State_MouseOver ||
        opt.state & QStyle::State_Selected) {
        opt.backgroundBrush = QBrush(QColor("#FF7009"));
        opt.palette.setColor(QPalette::Text, QColor("white"));
    } else {
        opt.backgroundBrush = QBrush(QColor("white"));
        opt.palette.setColor(QPalette::Text, QColor("#333333"));
    }

    QStyledItemDelegate::paint(painter, opt, index);
}

/**
 * @brief Constructs and initializes the MeasureNowWidget
 * @param parent The parent widget
 */
MeasureNowWidget::MeasureNowWidget(QWidget* parent)
    : QWidget(parent),
      deviceController(nullptr),
      countdownTimer(nullptr),
      remainingTime(0),
      currentScanPage(1),
      measurementDone(false),
      scanInProgress(false) {
    INFO("Initializing MeasureNowWidget");

    try {
        // Create and validate main layout
        auto* mainLayout = new QVBoxLayout(this);
        if (!mainLayout) {
            throw std::runtime_error("Failed to create main layout");
        }

        // Initialize and configure stacked widget
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
        countdownTimer->setInterval(1000);  // 1 second interval
        connect(countdownTimer, &QTimer::timeout, this,
                &MeasureNowWidget::updateCountdown);

        // Initialize and configure connection check timer
        connectionTimer = new QTimer(this);
        if (!connectionTimer) {
            throw std::runtime_error("Failed to create connection check timer");
        }
        connectionTimer->setInterval(1000);
        connect(connectionTimer, &QTimer::timeout, this, [this]() {
            if (!deviceController->isConnected() &&
                stackedWidget->currentIndex() != 0) {
                DEBUG("Device disconnected - resetting measurement");
                showAlert("Device disconnected - measurement cancelled");
                resetMeasurement();
            }
        });

        // Setup measurement data and pages
        initImagePaths();
        setupPages();

        DEBUG("MeasureNowWidget initialization completed successfully");
    } catch (const std::exception& e) {
        ERROR("Failed to initialize MeasureNowWidget:" << e.what());
    }
}

/**
 * @brief Sets and connects the device controller
 * @param controller Pointer to the device controller
 */
void MeasureNowWidget::setDeviceController(DeviceController* controller) {
    deviceController = controller;
    if (deviceController) {
        connect(deviceController, &DeviceController::dataReceived, this,
                &MeasureNowWidget::receiveData);
        DEBUG("DeviceController connected successfully");
    } else {
        ERROR("DeviceController is null in setDeviceController");
    }
}

void MeasureNowWidget::setUserId(int userId) {
    DEBUG(QString("Setting user ID: %1").arg(userId));
    currentUserId = userId;
    if (profileComboBox && profileController) {
        populateProfileList(profileComboBox);
    }
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
        // Initialize and configure start/stop button
        startStopButton = new QPushButton("Start Measurement");
        if (!startStopButton) {
            throw std::runtime_error("Failed to create start/stop button");
        }
        startStopButton->setStyleSheet(BUTTON_STYLE);
        startStopButton->setMinimumHeight(40);
        startStopButton->setFixedWidth(200);

        if (!connect(startStopButton, &QPushButton::clicked, this,
                     &MeasureNowWidget::onStartStopButtonClicked)) {
            WARNING("Failed to connect start/stop button signal");
        }

        // Initialize alert container and layout
        auto* alertContainer = new QWidget;
        alertContainer->setFixedHeight(45);
        auto* alertLayout = new QHBoxLayout(alertContainer);
        alertLayout->setContentsMargins(0, 0, 0, 0);

        // Configure alert label with styling
        alertLabel = new QLabel("");
        alertLabel->setStyleSheet(
            "QLabel { "
            "    font-size: 11px; "
            "    color: white; "
            "    background-color: #FF4444; "
            "    border-radius: 15px; "
            "    padding: 8px 25px; "
            "    height: 30px; "
            "    margin: 0px; "
            "}");
        alertLabel->setAlignment(Qt::AlignCenter);
        alertLabel->setWordWrap(false);
        alertLabel->setFixedHeight(30);
        alertLabel->setVisible(false);

        alertLayout->addWidget(alertLabel, 0, Qt::AlignCenter);

        // Add components to main layout
        mainLayout->addStretch(1);
        mainLayout->addWidget(alertContainer, 0, Qt::AlignCenter);
        mainLayout->addWidget(startStopButton, 0, Qt::AlignCenter);

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
        // Create intro page
        createIntroPage();

        // Create measurement pages
        for (int i = 1; i <= TOTAL_SCAN_PAGES; ++i) {
            createScanPage(i);
            DEBUG("Created scan page " << i);
        }

        // Create results page
        createResultsPage();

        INFO("Successfully set up all pages");
    } catch (const std::exception& e) {
        ERROR("Failed to setup pages: " << e.what());
    }
}

void MeasureNowWidget::setUserProfileController(
    UserProfileController* controller) {
    profileController = controller;
}

/**
 * @brief Creates the introduction page with setup instructions
 */
void MeasureNowWidget::createIntroPage() {
    DEBUG("Creating intro page");

    // Setup main page layout
    auto* introPage = new QWidget;
    auto* layout = new QVBoxLayout(introPage);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(30);

    // Create main container with styling
    auto* contentContainer = new QWidget;
    contentContainer->setObjectName("contentContainer");
    contentContainer->setStyleSheet(
        "#contentContainer {"
        "    background-color: white;"
        "    border-radius: 10px;"
        "}");

    auto* containerLayout = new QVBoxLayout(contentContainer);
    containerLayout->setSpacing(25);
    containerLayout->setContentsMargins(30, 30, 30, 30);

    // Setup header section
    auto* headerContainer = new QWidget;
    auto* headerLayout = new QHBoxLayout(headerContainer);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    auto* headerLabel = new QLabel("RaDoTech Simulation");
    headerLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: bold;"
        "color: #333333;");
    headerLayout->addWidget(headerLabel, 0, Qt::AlignCenter);

    // Setup introduction section
    auto* introContainer = new QWidget;
    introContainer->setStyleSheet(
        "background-color: #F8F9FA;"
        "border-radius: 10px;"
        "padding: 20px;");

    auto* introLayout = new QVBoxLayout(introContainer);

    auto* introTitle = new QLabel("Before You Begin:");
    introTitle->setStyleSheet(
        "font-size: 16px;"
        "font-weight: bold;"
        "color: #FF7009;");

    auto* introText = new QLabel(
        "• Ensure your device is powered on and connected\n"
        "• Hold the device steady at each measurement point\n"
        "• Wait for the measurement to complete\n"
        "• Complete all 24 measurement points in sequence\n"
        "• Tap the battery percentage to recharge if needed");
    introText->setStyleSheet(
        "font-size: 15px;"
        "color: #555555;"
        "line-height: 1.8;");
    introText->setWordWrap(true);

    introLayout->addWidget(introTitle);
    introLayout->addWidget(introText);

    // Setup form section
    auto* formContainer = new QWidget;
    auto* formLayout = new QFormLayout(formContainer);
    formLayout->setSpacing(15);
    formLayout->setContentsMargins(0, 0, 0, 0);

    // Create and setup form controls
    profileComboBox = new QComboBox();
    profileComboBox->setStyleSheet(COMBOBOX_STYLE);
    profileComboBox->setItemDelegate(new BackgroundDelegate(profileComboBox));

    // Add profile selection changed handler
    connect(
        profileComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [this](int index) {
            selectedProfileId = profileComboBox->itemData(index).toInt();
            DEBUG(QString("Selected profile ID: %1").arg(selectedProfileId));
        });

    populateProfileList(profileComboBox);

    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setStyleSheet(INPUT_STYLE);

    // Add form fields with styled labels
    QString labelStyle = "font-weight: bold; color: #333333; font-size: 14px;";
    auto addFormRow = [&](const QString& labelText, QWidget* widget) {
        auto* label = new QLabel(labelText);
        label->setStyleSheet(labelStyle);
        formLayout->addRow(label, widget);
    };

    addFormRow("Profile:", profileComboBox);
    addFormRow("Date:", dateEdit);

    // Assemble all sections
    containerLayout->addWidget(headerContainer);
    containerLayout->addWidget(introContainer);
    containerLayout->addWidget(formContainer);
    containerLayout->addStretch();

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
    auto* layout = new QVBoxLayout(scanPage);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);

    // Calculate side and image index
    bool isRightSide = pageNum > MEASUREMENTS_PER_SIDE;
    int imageIndex =
        isRightSide ? (pageNum - MEASUREMENTS_PER_SIDE - 1) : (pageNum - 1);

    // Create and style page header
    QString sideText = isRightSide ? "Right" : "Left";
    auto* pageLabel = new QLabel(
        QString("Scan Point %1 of %2 (%3 Side)")
            .arg(isRightSide ? pageNum - MEASUREMENTS_PER_SIDE : pageNum)
            .arg(MEASUREMENTS_PER_SIDE)
            .arg(sideText));
    pageLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 20px; "
        "    color: #333333; "
        "    font-weight: bold; "
        "}");
    pageLabel->setAlignment(Qt::AlignCenter);

    // Create status indicator container
    auto* statusContainer = new QWidget;
    statusContainer->setStyleSheet(
        "QWidget { "
        "    background-color: #F5F5F5; "
        "    border-radius: 15px; "
        "    padding: 8px 20px; "
        "}");
    statusContainer->setFixedHeight(45);

    auto* statusLayout = new QHBoxLayout(statusContainer);
    statusLayout->setContentsMargins(15, 0, 15, 0);

    // Create and style status label
    auto* statusLabel = new QLabel("Place device on measurement point");
    statusLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 16px; "
        "    color: #666666; "
        "}");
    statusLabel->setAlignment(Qt::AlignCenter);
    countdownLabels.append(statusLabel);

    statusLayout->addWidget(statusLabel);

    // Custom image label class for handling resizing
    class ResizableImageLabel : public QLabel {
       public:
        explicit ResizableImageLabel(QWidget* parent = nullptr)
            : QLabel(parent) {
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            setMinimumSize(600, 400);
            setAlignment(Qt::AlignCenter);
        }

        void setOriginalPixmap(const QPixmap& pixmap) {
            originalPixmap = pixmap;
            updatePixmap();
        }

       protected:
        void resizeEvent(QResizeEvent* event) override {
            QLabel::resizeEvent(event);
            updatePixmap();
        }

       private:
        QPixmap originalPixmap;
        void updatePixmap() {
            if (!originalPixmap.isNull()) {
                setPixmap(originalPixmap.scaled(
                    size() * 0.95,  // 95% of available space
                    Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    };

    // Setup measurement point image
    auto* imageLabel = new ResizableImageLabel();

    if (imageIndex >= 0 && imageIndex < imagePaths.size()) {
        QPixmap originalPixmap(imagePaths.at(imageIndex));
        if (!originalPixmap.isNull()) {
            if (isRightSide) {
                // Mirror image for right side measurements
                originalPixmap =
                    originalPixmap.transformed(QTransform().scale(-1, 1));
            }
            imageLabel->setOriginalPixmap(originalPixmap);
        } else {
            WARNING("Failed to load image for scan page " << pageNum);
        }
    }

    // Assemble page layout
    layout->addWidget(pageLabel);
    layout->addWidget(imageLabel, 1);
    layout->addWidget(statusContainer);

    stackedWidget->addWidget(scanPage);
    DEBUG("Scan page " << pageNum << " created successfully");
}

/**
 * @brief Creates the results page placeholder
 */
void MeasureNowWidget::createResultsPage() {
    DEBUG("Creating results page");

    auto* resultsPage = new QWidget;
    auto* layout = new QVBoxLayout(resultsPage);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    stackedWidget->addWidget(resultsPage);
    DEBUG("Results page created successfully");
}

/**
 * @brief Initiates the measurement countdown process
 */
void MeasureNowWidget::startCountdown() {
    // Validate current page and state
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

    // Update UI for measurement
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

    // Store measurement data
    ScanPoint scanPoint{currentPage, data};
    rawMeasurements.append(scanPoint);
    DEBUG("Total measurements collected: " << rawMeasurements.size());

    showMeasurementResult(data);
}

/**
 * @brief Processes all collected measurements and calculates final results
 * TODO: Configure where the measurements are processed and stored to database
 */
void MeasureNowWidget::processMeasurements() {
    if (selectedProfileId == -1) {
        WARNING("No profile selected");
        return;
    }

    DEBUG("Processing measurements");
    DEBUG("Total raw measurements: " << rawMeasurements.size());
    DEBUG("Number of measurement labels: " << measurementLabels.size());

    calculatedResults.clear();

    // Log raw measurements for debugging
    for (const auto& scan : rawMeasurements) {
        DEBUG("Raw measurement - Position: " << scan.position
                                             << ", Value: " << scan.rawValue);
    }

    // Calculate results for each measurement area
    for (int i = 0; i < MEASUREMENTS_PER_SIDE; ++i) {
        int leftValue = -1;
        int rightValue = -1;

        // Find corresponding left and right measurements
        for (const auto& scan : rawMeasurements) {
            if (scan.position == i + 1) {  // Left side (1-12)
                leftValue = scan.rawValue;
            }
            if (scan.position ==
                i + MEASUREMENTS_PER_SIDE + 1) {  // Right side (13-24)
                rightValue = scan.rawValue;
            }
        }

        // TODO: Determine proper values using calculator
        // Calculate and store mean value
        if (leftValue != -1 && rightValue != -1) {
            int meanValue = (leftValue + rightValue) / 2;
            QString label = measurementLabels[i];
            calculatedResults[label] = meanValue;
            DEBUG("Processed " << label << ": Left=" << leftValue << ", Right="
                               << rightValue << ", Mean=" << meanValue);
        } else {
            DEBUG("Missing measurement for point "
                  << (i + 1) << " (Left=" << leftValue
                  << ", Right=" << rightValue << ")");
        }
    }

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

        // Start connection monitoring when beginning measurements
        if (connectionTimer) {
            connectionTimer->start();
        }
        nextPage();
    } else {
        // Stop monitoring when returning to intro page
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
    // Validate device state
    if (!deviceController || !deviceController->isDeviceOn() ||
        !deviceController->isConnected()) {
        return;
    }

    // Verify current page is a scan page
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
        // Process measurements before showing results page
        if (nextIndex == stackedWidget->count() - 1) {
            DEBUG("About to show results page, processing measurements");
            processMeasurements();
        }

        stackedWidget->setCurrentIndex(nextIndex);
        currentScanPage++;
        updateButtonState();
    }
}

/**
 * @brief Resets the measurement process to initial state
 */
void MeasureNowWidget::resetMeasurement() {
    DEBUG("Resetting measurement");

    // Stop connection monitoring when resetting
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

    // Reset status label text
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
    // Check if device disconnected during measurement
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
 *
 * TODO: Incorporate the profile controller
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

    // Clean up
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
 * @brief Displays the measurement results in the results page
 * TODO: Possibly create a separate class for the results page
 */
void MeasureNowWidget::displayResults() {
    DEBUG("Displaying results");
    DEBUG("Number of calculated results: " << calculatedResults.size());

    // Log individual results for debugging
    for (auto it = calculatedResults.begin(); it != calculatedResults.end();
         ++it) {
        DEBUG("Result - " << it.key() << ": " << it.value());
    }

    // Create debug string list for comprehensive logging
    QStringList debugResults;
    for (int i = 0; i < measurementLabels.size(); ++i) {
        QString label = measurementLabels[i];
        int value = calculatedResults.value(label, 0);
        debugResults.append(QString("%1: %2").arg(label).arg(value));
    }

    updateButtonState();

    // Get results page widget
    auto* resultsPage = qobject_cast<QWidget*>(
        stackedWidget->widget(stackedWidget->count() - 1));
    if (!resultsPage) {
        ERROR("Results page not found");
        return;
    }

    // Get results layout
    auto* layout = resultsPage->findChild<QVBoxLayout*>();
    if (!layout) {
        ERROR("Results layout not found");
        return;
    }

    // Clear existing results
    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Create results header
    QLabel* resultsLabel = new QLabel("Scan Results");
    resultsLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 24px; "
        "    color: #333333; "
        "    font-weight: bold; "
        "}");
    resultsLabel->setAlignment(Qt::AlignCenter);

    // Setup two-column layout
    QHBoxLayout* columnsLayout = new QHBoxLayout;
    QVBoxLayout* leftColumn = new QVBoxLayout;
    QVBoxLayout* rightColumn = new QVBoxLayout;

    int itemsPerColumn = (measurementLabels.size() + 1) / 2;

    // Create result widgets for each measurement
    for (int i = 0; i < measurementLabels.size(); ++i) {
        QString label = measurementLabels[i];
        int value = calculatedResults.value(label, 0);

        // Create and style result widget
        QWidget* resultWidget = new QWidget;

        // Determine background color based on value
        // TODO: Adjust colours based on scan results and normal point ranges
        QString bgColor;
        if (value > 70) {
            bgColor = "#4CAF50";
        } else if (value >= 31) {
            bgColor = "#FF8001";
        } else {
            bgColor = "#D32F2F";
        }

        resultWidget->setStyleSheet(QString("QWidget { "
                                            "    background-color: %1; "
                                            "    border-radius: 8px; "
                                            "}")
                                        .arg(bgColor));

        // Setup result layout
        auto* resultLayout = new QHBoxLayout(resultWidget);
        resultLayout->setContentsMargins(12, 8, 12, 8);
        resultLayout->setSpacing(10);

        // Create and style point name label
        auto* pointLabel = new QLabel(label);
        pointLabel->setStyleSheet(
            "QLabel { "
            "    font-size: 13px; "
            "    color: white; "
            "    font-weight: bold; "
            "}");
        pointLabel->setMinimumWidth(150);
        pointLabel->setWordWrap(true);

        // Create value display
        auto* valueContainer = new QWidget;
        valueContainer->setFixedWidth(40);
        auto* valueLayout = new QHBoxLayout(valueContainer);
        valueLayout->setContentsMargins(0, 0, 0, 0);

        auto* valueLabel = new QLabel(QString::number(value));
        valueLabel->setStyleSheet(
            "QLabel { "
            "    font-size: 15px; "
            "    color: white; "
            "    font-weight: bold; "
            "}");
        valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // Assemble result widget
        valueLayout->addWidget(valueLabel);
        resultLayout->addWidget(pointLabel);
        resultLayout->addStretch();
        resultLayout->addWidget(valueContainer);
        resultWidget->setFixedHeight(40);

        // Add to appropriate column
        if (i < itemsPerColumn) {
            leftColumn->addWidget(resultWidget);
        } else {
            rightColumn->addWidget(resultWidget);
        }
    }

    columnsLayout->addLayout(leftColumn);
    columnsLayout->addLayout(rightColumn);

    // Add timestamp
    auto* timestampLabel = new QLabel(
        QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    timestampLabel->setStyleSheet(
        "QLabel { "
        "    color: #999999; "
        "    font-size: 12px; "
        "}");
    timestampLabel->setAlignment(Qt::AlignRight);

    // Assemble final layout
    layout->addWidget(resultsLabel);
    layout->addLayout(columnsLayout);
    layout->addWidget(timestampLabel);
}

/**
 * @brief Handles errors during the scanning process
 */
void MeasureNowWidget::handleScanError() {
    DEBUG("Handling scan error");

    countdownTimer->stop();

    // Reset status label
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

void MeasureNowWidget::refreshProfiles() {
    if (profileComboBox && profileController) {
        DEBUG("Refreshing profile list");
        populateProfileList(profileComboBox);
    }
}
