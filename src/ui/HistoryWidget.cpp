/**
 * @file HistoryWidget.cpp
 * @brief Implementation of the HistoryWidget class.
 */

#include "HistoryWidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
// TODO: Remove after implementation
#include <QRandomGenerator>
// ---

HistoryWidget::HistoryWidget(QWidget *parent) : QWidget(parent) {
    const int columns = 2;
    const QStringList metricNames = {"Energy Level", "Immune System",
                                     "Metabolism", "Psycho-emotional State",
                                     "Musculoskeletal System"};

    // Create the grid layout
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setSpacing(30);
    gridLayout->setContentsMargins(40, 40, 40, 40);

    // TODO: Implement retreiving of trends from historical scans
    QList<int> trendValues = {85, 50, 65, 30, 95};

    // Import icons from resources
    QString trendingUpIconPath = ":/icons/trending_up.png";
    QString trendingDownIconPath = ":/icons/trending_down.png";
    QString levelIconPath = ":/icons/level.png";

    // Create trend widgets
    for (int i = 0; i < metricNames.size(); ++i) {
        QWidget *trendWidget = new QWidget;
        trendWidget->setStyleSheet("border-radius: 15px;");
        trendWidget->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Expanding);  // Allow widgets to expand equally

        // TODO: Determine normal values for each trend
        // Determine the background color based on the trend value
        QString bgColor;
        int value = trendValues[i];
        if (value > 70) {
            bgColor = "#D32F2F";
        } else if (value >= 31 && value <= 70) {
            bgColor = "#4CAF50";
        } else {
            bgColor = "#7067DD";
        }

        // Set the background color
        trendWidget->setStyleSheet(
            QString("background-color: %1; border-radius: 15px;").arg(bgColor));

        // Create a layout for the trend widget
        QGridLayout *trendLayout = new QGridLayout(trendWidget);
        trendLayout->setContentsMargins(15, 15, 15, 15);
        trendLayout->setSpacing(5);

        // Trend indicator image (top-left)
        QLabel *trendImageLabel = new QLabel;
        trendImageLabel->setFixedSize(40, 40);

        // Determine the trend indicator image based on the value
        QString trendIconPath;
        int previousValue =
            value - QRandomGenerator::global()->bounded(-10, 10);
        if (value > previousValue + 2) {
            trendIconPath = trendingUpIconPath;
        } else if (value < previousValue - 2) {
            trendIconPath = trendingDownIconPath;
        } else {
            trendIconPath = levelIconPath;
        }
        QPixmap trendPixmap(trendIconPath);
        trendImageLabel->setPixmap(trendPixmap.scaled(
            40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Numerical value label (top-right)
        QLabel *valueLabel = new QLabel(QString::number(value));
        valueLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
        valueLabel->setStyleSheet(
            "font-size: 36px; color: white; font-weight: bold;");

        // Metric name label (bottom-left)
        QLabel *metricLabel = new QLabel(metricNames[i]);
        metricLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
        metricLabel->setStyleSheet(
            "font-size: 20px; color: white; font-weight: bold;");

        // Add widgets to the trend layout
        trendLayout->addWidget(trendImageLabel, 0, 0,
                               Qt::AlignLeft | Qt::AlignTop);
        trendLayout->addWidget(valueLabel, 0, 1, Qt::AlignRight | Qt::AlignTop);
        trendLayout->addWidget(metricLabel, 1, 0, 1, 2,
                               Qt::AlignLeft | Qt::AlignBottom);

        trendLayout->setRowStretch(0, 1);
        trendLayout->setRowStretch(1, 1);
        trendLayout->setColumnStretch(0, 1);
        trendLayout->setColumnStretch(1, 1);

        // Add the trend widget to the grid layout
        int row = i / columns;
        int column = i % columns;
        gridLayout->addWidget(trendWidget, row, column);
    }

    // Create the "View Previous Scans" button as the sixth widget
    QPushButton *viewPreviousScansButton =
        new QPushButton("View Previous Scans");
    viewPreviousScansButton->setStyleSheet(
        "background-color: #FF8001; border-radius: 15px; font-size: 20px; "
        "color: white; font-weight: bold;");
    viewPreviousScansButton->setSizePolicy(QSizePolicy::Expanding,
                                           QSizePolicy::Expanding);

    connect(viewPreviousScansButton, &QPushButton::clicked, this,
            &HistoryWidget::onViewPreviousScansButtonClicked);

    // Add the button to the grid layout
    int row = metricNames.size() / columns;
    int column = metricNames.size() % columns;
    gridLayout->addWidget(viewPreviousScansButton, row, column);

    // Set row and column stretch factors to ensure equal sizing
    int totalRows = gridLayout->rowCount();
    for (int r = 0; r < totalRows; ++r) {
        gridLayout->setRowStretch(r, 1);
    }
    for (int c = 0; c < columns; ++c) {
        gridLayout->setColumnStretch(c, 1);
    }

    // Set the layout to the widget
    setLayout(gridLayout);
}

// Slot to handle the "View Previous Scans" button click
void HistoryWidget::onViewPreviousScansButtonClicked() {
    // TODO: Implement the logic to show the previous scans
}
