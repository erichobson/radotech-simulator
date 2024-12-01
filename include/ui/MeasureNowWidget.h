/**
 * @file MeasureNowWidget.h
 * @brief Widget for managing the measurement process and results display
 */

#ifndef MEASURENOWWIDGET_H
#define MEASURENOWWIDGET_H

#include <QStyledItemDelegate>
#include <QWidget>

// Forward declarations
class QStackedWidget;
class QLabel;
class QPushButton;
class QDateEdit;
class QTimeEdit;
class QTimer;
class QComboBox;
class QVBoxLayout;
class QFormLayout;
class DeviceController;

/**
 * @brief Custom delegate for styling background in combo boxes
 */
class BackgroundDelegate : public QStyledItemDelegate {
    Q_OBJECT

   public:
    explicit BackgroundDelegate(QObject* parent = nullptr);
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
};

/**
 * @brief Widget for managing the measurement process and results display
 *
 * This widget handles the complete measurement workflow including:
 * - Device connection and control
 * - Measurement process for left and right sides
 * - Results calculation and display
 */
class MeasureNowWidget : public QWidget {
    Q_OBJECT

   public:
    explicit MeasureNowWidget(QWidget* parent = nullptr);
    void setDeviceController(DeviceController* controller);

   public slots:
    void startCountdown();
    void measurementComplete();
    void receiveData(int data);
    void onImageReleased();
    void onStartStopButtonClicked();

   private:
    // Constants and Types
    static const int TOTAL_SCAN_PAGES = 24;
    static const int MEASUREMENTS_PER_SIDE = 12;

    struct ScanPoint {
        int position;
        int rawValue;
    };

    // Measurement Data
    const QStringList measurementLabels = {"Lungs",
                                           "Pericardium",
                                           "Heart",
                                           "Small Intestine",
                                           "Immune System",
                                           "Large Intestine",
                                           "Pancreas and Spleen",
                                           "Liver",
                                           "Kidney",
                                           "Bladder",
                                           "Gallbladder",
                                           "Stomach"};
    QStringList imagePaths;
    QVector<ScanPoint> rawMeasurements;
    QMap<QString, int> calculatedResults;

    // UI Components
    QStackedWidget* stackedWidget{nullptr};
    QList<QLabel*> countdownLabels;
    QPushButton* startStopButton{nullptr};
    QLabel* alertLabel{nullptr};
    QDateEdit* dateEdit{nullptr};
    QTimeEdit* timeEdit{nullptr};

    // Device and State
    DeviceController* deviceController{nullptr};
    QTimer* countdownTimer{nullptr};
    QTimer* connectionTimer{nullptr};
    int remainingTime{0};
    int currentScanPage{1};
    bool measurementDone{false};
    bool scanInProgress{false};

    // Initialization
    void initializeUIComponents(QVBoxLayout* mainLayout);
    void initImagePaths();
    void setupPages();

    // Page Management
    void createIntroPage();
    void createScanPage(int pageNum);
    void createResultsPage();
    void nextPage();

    // Measurement Processing
    void processMeasurements();
    bool areAllMeasurementsComplete() const;
    void showMeasurementResult(int value);
    void displayResults();

    // State Management
    void resetMeasurement();
    void resetState();
    void updateButtonState();
    void updateCountdown();

    // UI Helpers
    void populateProfileList(QComboBox* profileComboBox);
    void setErrorState(QLabel* label);
    void handleScanError();
    void showAlert(const QString& message, bool autoHide = true);
};

#endif  // MEASURENOWWIDGET_H
