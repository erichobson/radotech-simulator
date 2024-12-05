/**
 * @file ResultsWidget.cpp
 * @brief
 */

#include "ResultsWidget.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

#include "Logging.h"

ResultsWidget::ResultsWidget(QWidget* parent)
    : QWidget(parent), mainLayout(new QVBoxLayout(this)) {
    DEBUG("Initializing ResultsWidget");
    setupUI();
}

/**
 * @brief
 */
void ResultsWidget::setupUI() {
    DEBUG("Setting up UI");

    mainLayout->setContentsMargins(0, 20, 0, 20);
    mainLayout->setSpacing(20);
    setLayout(mainLayout);

    QWidget* cardWidget = new QWidget;
    cardWidget->setStyleSheet("background-color: white; border-radius: 10px;");
    QVBoxLayout* cardLayout = new QVBoxLayout(cardWidget);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(30, 30, 30, 30);

    mainLayout->addWidget(cardWidget);
    this->cardLayout = cardLayout;
}

/**
 * @brief
 */
void ResultsWidget::displayResults() {
    DEBUG("Displaying results");

    // Clear existing widgets from card layout
    QLayoutItem* child;
    while ((child = cardLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Add back button only if needed (for history view)
    if (showBackButton) {
        QPushButton* backButton = new QPushButton("Back");
        backButton->setFixedSize(60, 30);
        backButton->setStyleSheet(
            "QPushButton {"
            "    background-color: transparent;"
            "    color: #333333;"
            "    font-size: 14px;"
            "    border: none;"
            "}"
            "QPushButton:hover {"
            "    color: #666666;"
            "}");
        cardLayout->addWidget(backButton, 0, Qt::AlignLeft);

        connect(backButton, &QPushButton::clicked, this,
                &ResultsWidget::backButtonClicked);
    }

    // Create header section
    QWidget* headerWidget = new QWidget;
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    // Title and Date
    QVBoxLayout* titleLayout = new QVBoxLayout;

    QLabel* titleLabel = new QLabel("Scan Results");
    titleLabel->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: #333333;");

    QLabel* dateLabel =
        new QLabel(currentScan.getCreatedOn().toString("MMMM d, yyyy"));
    dateLabel->setStyleSheet("font-size: 16px; color: #666666;");

    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(dateLabel);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();

    // Add header to card layout
    cardLayout->addWidget(headerWidget);

    // Create vital signs section
    QWidget* vitalsWidget = new QWidget;
    vitalsWidget->setStyleSheet(
        "background-color: #F8F8F8; "
        "border-radius: 10px;");

    QHBoxLayout* vitalsLayout = new QHBoxLayout(vitalsWidget);
    vitalsLayout->setSpacing(40);

    // Create vital sign items
    QList<QPair<QString, QString>> vitals = {
        {"Temperature",
         QString("%1°C").arg(static_cast<double>(currentScan.getBodyTemp()), 0,
                             'f', 1)},
        {"Heart Rate", QString("%1 bpm").arg(currentScan.getHeartRate())},
        {"Blood Pressure",
         QString("%1 mmHg").arg(currentScan.getBloodPressure())},
        {"Weight",
         QString("%1 kg").arg(
             static_cast<double>(currentScan.getCurrentWeight()), 0, 'f', 1)}};

    for (const auto& vital : vitals) {
        QVBoxLayout* vitalItemLayout = new QVBoxLayout;

        QLabel* valueLabel = new QLabel(vital.second);
        valueLabel->setStyleSheet(
            "font-size: 22px; font-weight: bold; color: #333333;");

        QLabel* nameLabel = new QLabel(vital.first);
        nameLabel->setStyleSheet("font-size: 14px; color: #666666;");

        vitalItemLayout->addWidget(valueLabel);
        vitalItemLayout->addWidget(nameLabel);
        vitalsLayout->addLayout(vitalItemLayout);
    }

    vitalsLayout->addStretch();
    cardLayout->addWidget(vitalsWidget);

    // Create metrics section
    QWidget* metricsWidget = new QWidget;
    QVBoxLayout* metricsLayout = new QVBoxLayout(metricsWidget);
    metricsLayout->setSpacing(10);

    // Calculate metrics
    healthMetrics.clear();
    if (!calculator.calculateOrganHealth(&currentScan, healthMetrics)) {
        ERROR("Failed to calculate organ health metrics");
        return;
    }

    QGridLayout* metricsGrid = new QGridLayout;
    metricsGrid->setSpacing(20);
    int row = 0;
    int col = 0;
    const int COLS = 2;

    for (const auto& metric : healthMetrics) {
        // Create metric card
        QWidget* metricCard = new QWidget;
        metricCard->setFixedHeight(35);  // Compact height

        // TODO: Fix colours
        QString statusColor = metric->getLevel() > 0   ? "#FF8001"
                              : metric->getLevel() < 0 ? "#D32F2F"
                                                       : "#4CAF50";
        // Create layout for the card
        QHBoxLayout* cardLayout = new QHBoxLayout(metricCard);
        cardLayout->setContentsMargins(0, 0, 0, 0);
        cardLayout->setSpacing(10);  // Consistent spacing

        // Create labels
        QLabel* nameLabel = new QLabel(metric->getName());
        nameLabel->setStyleSheet(
            "font-size: 15px; "
            "color: #333333;");

        QLabel* valueLabel =
            new QLabel(QString::number(metric->getValue(), 'f', 1));
        valueLabel->setStyleSheet(QString("font-size: 15px; "
                                          "font-weight: bold; "
                                          "color: %1;")
                                      .arg(statusColor));

        // Set alignment
        nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        valueLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // Set size policies
        nameLabel->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::Preferred);
        valueLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        valueLabel->setMinimumWidth(50);

        // Add labels to layout
        cardLayout->addWidget(nameLabel);
        cardLayout->addWidget(valueLabel);

        // Add to grid layout
        metricsGrid->addWidget(metricCard, row, col);

        // Update position
        col++;
        if (col >= COLS) {
            col = 0;
            row++;
        }
    }

    // Add the grid to the metrics layout
    metricsLayout->addLayout(metricsGrid);
    cardLayout->addWidget(metricsWidget);

    cardLayout->addStretch();
}

/**
 * @brief
 *
 * @param scanModel
 */
void ResultsWidget::setScanModel(const ScanModel& scanModel) {
    DEBUG("Setting scan model");
    currentScan = scanModel;
    displayResults();
}

/**
 * @brief
 */
void ResultsWidget::displayGeneralHealthParameters() {
    DEBUG("Starting to display general health parameters");

    // Log all parameters
    INFO(QString("Body Temperature: %1").arg(currentScan.getBodyTemp()));
    INFO(QString("Blood Pressure: %1").arg(currentScan.getBloodPressure()));
    INFO(QString("Heart Rate: %1").arg(currentScan.getHeartRate()));
    INFO(QString("Sleeping Time: %1").arg(currentScan.getSleepingTime()));
    INFO(QString("Current Weight: %1").arg(currentScan.getCurrentWeight()));
    INFO(QString("Emotional State: %1").arg(currentScan.getEmotionalState()));
    INFO(QString("Overall Feeling: %1").arg(currentScan.getOverallFeeling()));

    // Get the values from currentScan
    double bodyTemp = currentScan.getBodyTemp();
    int bloodPressure = currentScan.getBloodPressure();
    int heartRate = currentScan.getHeartRate();
    double sleepingTime = currentScan.getSleepingTime();
    double currentWeight = currentScan.getCurrentWeight();
    int emotionalState = currentScan.getEmotionalState();
    int overallFeeling = currentScan.getOverallFeeling();

    QWidget* containerWidget = new QWidget;
    containerWidget->setMinimumHeight(300);
    QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget);

    QGroupBox* generalHealthGroup = new QGroupBox("General Health Parameters");
    generalHealthGroup->setSizePolicy(QSizePolicy::Expanding,
                                      QSizePolicy::Minimum);
    generalHealthGroup->setMinimumHeight(250);
    generalHealthGroup->setStyleSheet(
        "QGroupBox { "
        "    font-size: 20px; "
        "    font-weight: bold; "
        "    color: #333333; "
        "    border: 2px solid #E0E0E0; "
        "    border-radius: 10px; "
        "    margin-top: 30px; "
        "    background-color: #FFFFFF; "
        "    padding: 20px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    subcontrol-position: top center; "
        "    padding: 0 10px; "
        "    background-color: white; "
        "}");

    QGridLayout* layout = new QGridLayout(generalHealthGroup);
    layout->setSpacing(15);

    QList<QPair<QString, QString>> parameters = {
        {"Body Temperature", QString("%1 °C").arg(bodyTemp, 0, 'f', 1)},
        {"Blood Pressure", QString("%1 mmHg").arg(bloodPressure)},
        {"Heart Rate", QString("%1 bpm").arg(heartRate)},
        {"Sleeping Time", QString("%1 hours").arg(sleepingTime, 0, 'f', 1)},
        {"Current Weight", QString("%1 kg").arg(currentWeight, 0, 'f', 1)},
        {"Emotional State", QString("%1 / 5").arg(emotionalState)},
        {"Overall Feeling", QString("%1 / 5").arg(overallFeeling)}};

    QString labelStyle =
        "font-size: 16px; color: #333333; font-weight: bold; padding: 5px;";

    // Add parameters to the grid layout
    int row = 0;
    for (const auto& param : parameters) {
        QLabel* nameLabel = new QLabel(param.first + ":");
        nameLabel->setStyleSheet(labelStyle);
        nameLabel->setMinimumWidth(150);

        QLabel* valueLabel = new QLabel(param.second);
        valueLabel->setStyleSheet(labelStyle);
        valueLabel->setMinimumWidth(100);

        layout->addWidget(nameLabel, row, 0, Qt::AlignLeft);
        layout->addWidget(valueLabel, row, 1, Qt::AlignRight);

        row++;
    }

    layout->setContentsMargins(20, 20, 20, 20);
    layout->setVerticalSpacing(15);
    layout->setHorizontalSpacing(30);

    containerLayout->addWidget(generalHealthGroup);
    containerLayout->addStretch();

    mainLayout->addWidget(containerWidget);

    DEBUG("Finished displaying general health parameters");
}
