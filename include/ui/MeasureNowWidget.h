// MeasureNowWidget.h

#ifndef MEASURENOWWIDGET_H
#define MEASURENOWWIDGET_H

#include <QStackedWidget>
#include <QTimer>
#include <QWidget>

class QLabel;
class QPushButton;
class DeviceController;

class MeasureNowWidget : public QWidget {
    Q_OBJECT

   public:
    explicit MeasureNowWidget(QWidget *parent = nullptr);

    void setDeviceController(DeviceController *controller);

   public slots:
    void startCountdown();
    void cancelCountdown();
    void measurementComplete();
    void receiveData(int data);
    void onImageReleased();
    void onStartStopButtonClicked();

   private:
    QStackedWidget *stackedWidget;
    QList<QLabel *> countdownLabels;
    QTimer *countdownTimer;
    QPushButton *startStopButton;
    QLabel *alertLabel;
    int remainingTime;
    int currentScanPage;
    static const int TOTAL_SCAN_PAGES = 12;

    void setupPages();
    void createIntroPage();
    void createScanPage(int pageNum);
    void createResultsPage();
    void resetLabel();
    void updateCountdown();
    void nextPage();
    void showScanError();
    void initImagePaths();
    void resetMeasurement();
    void updateButtonState();
    void displayResults();

    QStringList imagePaths;
    QVector<int> measurementData;
    bool measurementDone;

    DeviceController *deviceController;
};

#endif  // MEASURENOWWIDGET_H
