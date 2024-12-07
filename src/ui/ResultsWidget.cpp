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

    // Clear existing widgets
    QLayoutItem* child;
    while ((child = cardLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Back button (if needed)
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

    // Header section
    QWidget* headerWidget = new QWidget;
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);

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
    cardLayout->addWidget(headerWidget);

    // Vitals section
    QWidget* vitalsWidget = new QWidget;
    vitalsWidget->setStyleSheet(
        "background-color: #F8F8F8; border-radius: 10px;");
    QHBoxLayout* vitalsLayout = new QHBoxLayout(vitalsWidget);
    vitalsLayout->setSpacing(40);

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

    // Metrics Container
    QWidget* metricsContainer = new QWidget;
    QHBoxLayout* metricsContainerLayout = new QHBoxLayout(metricsContainer);
    metricsContainerLayout->setSpacing(20);

    // Organ Health Section
    QWidget* organWidget = new QWidget;
    organWidget->setStyleSheet(
        "background-color: #F8F8F8; border-radius: 10px;");
    organWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QVBoxLayout* organLayout = new QVBoxLayout(organWidget);

    QLabel* organTitle = new QLabel("Organ Health");
    organTitle->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #333333;");
    organLayout->addWidget(organTitle);
    int row = 0, col = 0;

    healthMetrics.clear();
    if (calculator.calculateOrganHealth(&currentScan, healthMetrics)) {
        QGridLayout* organGrid = new QGridLayout;
        organGrid->setSpacing(10);
        for (const auto& metric : healthMetrics) {
            QWidget* metricCard = new QWidget;
            metricCard->setSizePolicy(QSizePolicy::Expanding,
                                      QSizePolicy::Preferred);
            QHBoxLayout* cardLayout = new QHBoxLayout(metricCard);
            cardLayout->setContentsMargins(0, 0, 0, 0);

            QString statusColor = metric->getLevel() > 0   ? "#FF8001"
                                  : metric->getLevel() < 0 ? "#D32F2F"
                                                           : "#4CAF50";

            QLabel* nameLabel = new QLabel(metric->getName());
            nameLabel->setStyleSheet("font-size: 15px; color: #333333;");
            nameLabel->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Preferred);
            nameLabel->setMinimumWidth(200);

            QLabel* valueLabel =
                new QLabel(QString::number(metric->getValue(), 'f', 1));
            valueLabel->setStyleSheet(
                QString("font-size: 15px; font-weight: bold; color: %1;")
                    .arg(statusColor));
            valueLabel->setMinimumWidth(50);
            valueLabel->setAlignment(Qt::AlignRight);

            cardLayout->addWidget(nameLabel);
            cardLayout->addSpacing(20);
            cardLayout->addWidget(valueLabel);

            organGrid->addWidget(metricCard, row, col);
            if (++col >= 2) {
                col = 0;
                row++;
            }
        }
        organLayout->addLayout(organGrid);
    }

    // Indicator Health Section
    QWidget* indicatorWidget = new QWidget;
    indicatorWidget->setStyleSheet(
        "background-color: #F8F8F8; border-radius: 10px;");
    indicatorWidget->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Preferred);
    QVBoxLayout* indicatorLayout = new QVBoxLayout(indicatorWidget);

    QLabel* indicatorTitle = new QLabel("Indicator Health");
    indicatorTitle->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #333333;");
    indicatorLayout->addWidget(indicatorTitle);

    QVector<HealthMetricModel*> indicatorMetrics;
    if (calculator.calculateIndicatorHealth(&currentScan, indicatorMetrics)) {
        QGridLayout* indicatorGrid = new QGridLayout;
        indicatorGrid->setSpacing(10);
        row = 0;
        col = 0;
        for (const auto& metric : indicatorMetrics) {
            QWidget* metricCard = new QWidget;
            metricCard->setSizePolicy(QSizePolicy::Expanding,
                                      QSizePolicy::Preferred);
            QHBoxLayout* cardLayout = new QHBoxLayout(metricCard);
            cardLayout->setContentsMargins(0, 0, 0, 0);

            QString statusColor = metric->getLevel() > 0   ? "#FF8001"
                                  : metric->getLevel() < 0 ? "#D32F2F"
                                                           : "#4CAF50";

            QLabel* nameLabel = new QLabel(metric->getName());
            nameLabel->setStyleSheet("font-size: 15px; color: #333333;");
            nameLabel->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Preferred);

            QLabel* valueLabel =
                new QLabel(QString::number(metric->getValue(), 'f', 1));
            valueLabel->setStyleSheet(
                QString("font-size: 15px; font-weight: bold; color: %1;")
                    .arg(statusColor));
            valueLabel->setMinimumWidth(50);
            valueLabel->setAlignment(Qt::AlignRight);

            cardLayout->addWidget(nameLabel);
            cardLayout->addSpacing(20);
            cardLayout->addWidget(valueLabel);

            indicatorGrid->addWidget(metricCard, row, col);
            if (++col >= 1) {
                col = 0;
                row++;
            }
        }
        indicatorLayout->addLayout(indicatorGrid);
    }

    metricsContainerLayout->addWidget(organWidget);
    metricsContainerLayout->addWidget(indicatorWidget);
    cardLayout->addWidget(metricsContainer);

    // Recommendations section
    QWidget* recommendationsWidget = new QWidget;
    recommendationsWidget->setStyleSheet(
        "background-color: #F8F8F8; border-radius: 10px; padding: 15px;");
    QHBoxLayout* recommendationsLayout = new QHBoxLayout(recommendationsWidget);

    QLabel* recommendationsTitle = new QLabel("Specialist Recommendations:");
    recommendationsTitle->setStyleSheet(
        "font-size: 16px; font-weight: bold; color: #333333;");
    QLabel* recommendationsText = new QLabel(
        "Please consult with your healthcare provider for personalized advice "
        "based on these results.");
    recommendationsText->setStyleSheet("font-size: 14px; color: #666666;");
    recommendationsText->setWordWrap(true);

    recommendationsLayout->addWidget(recommendationsTitle);
    recommendationsLayout->addWidget(recommendationsText, 1);

    cardLayout->addWidget(recommendationsWidget);
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
