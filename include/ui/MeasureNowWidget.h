/**
 * @file MeasureNowWidget.h
 * @brief Widget for managing the measurement process and results display
 */

#ifndef MEASURENOWWIDGET_H
#define MEASURENOWWIDGET_H

#include <QMap>
#include <QWidget>

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
class ResultsWidget;
class ScanController;
class UserProfileController;
class ProfileModel;
class QSpinBox;
class QDoubleSpinBox;
class QComboBox;

class MeasureNowWidget : public QWidget {
    Q_OBJECT

   public:
    explicit MeasureNowWidget(
        QWidget* parent = nullptr, DeviceController* deviceController = nullptr,
        UserProfileController* userProfileController = nullptr,
        ScanController* scanController = nullptr, int userId = -1);
    void setUserId(int userId);
    void refreshProfiles();
    QComboBox* getProfileSelector() const { return profileComboBox; }

   protected:
    void resizeEvent(QResizeEvent* event) override;

   signals:
    void profileSelected(int profileId, const QString& profileName);

   public slots:
    void startCountdown();
    void measurementComplete();
    void receiveData(int data);
    void onImageReleased();
    void onStartStopButtonClicked();

   private:
    UserProfileController* profileController;
    ScanController* scanController;
    QComboBox* profileComboBox;
    int selectedProfileId;
    int currentUserId;

    static const int TOTAL_SCAN_PAGES = 24;
    static const int MEASUREMENTS_PER_SIDE = 12;

    struct ScanPoint {
        int position;
        int rawValue;
    };

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
    ResultsWidget* resultsWidget;
    QMap<int, QPixmap> originalPixmaps;

    QStackedWidget* stackedWidget{nullptr};
    QList<QLabel*> countdownLabels;
    QPushButton* startStopButton{nullptr};
    QLabel* alertLabel{nullptr};
    QDateEdit* dateEdit{nullptr};
    QTimeEdit* timeEdit{nullptr};

    DeviceController* deviceController{nullptr};
    QTimer* countdownTimer{nullptr};
    QTimer* connectionTimer{nullptr};
    int remainingTime{0};
    int currentScanPage{1};
    bool measurementDone{false};
    bool scanInProgress{false};

    QDoubleSpinBox* bodyTempEdit;
    QSpinBox* bloodPressureEdit;
    QSpinBox* heartRateEdit;
    QDoubleSpinBox* sleepingTimeEdit;
    QDoubleSpinBox* currentWeightEdit;
    QComboBox* emotionalStateEdit;
    QComboBox* overallFeelingEdit;

    double bodyTemp;
    int bloodPressure;
    int heartRate;
    double sleepingTime;
    double currentWeight;
    int emotionalState;
    int overallFeeling;

    void collectUserInputs();
    void adjustImageSize(int pageIndex);

    void initializeUIComponents(QVBoxLayout* mainLayout);
    void initImagePaths();
    void setupPages();

    void createIntroPage();
    void createScanPage(int pageNum);
    void createPostScanInputPage();
    void createResultsPage();
    void nextPage();

    void processMeasurements();
    bool areAllMeasurementsComplete() const;
    void showMeasurementResult(int value);
    void displayResults();

    void resetMeasurement();
    void resetState();
    void updateButtonState();
    void updateCountdown();

    void populateProfileList(QComboBox* profileComboBox);
    void setErrorState(QLabel* label);
    void handleScanError();
    void showAlert(const QString& message, bool autoHide = true);
};

#endif  // MEASURENOWWIDGET_H
