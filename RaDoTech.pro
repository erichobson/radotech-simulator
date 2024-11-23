QT += core gui widgets sql

CONFIG += c++17
CONFIG -= app_bundle

TEMPLATE = app
TARGET = RaDoTech

BUILD_DIR = $$OUT_PWD
OBJECTS_DIR = $$BUILD_DIR/obj
MOC_DIR = $$BUILD_DIR/moc
UI_DIR = $$BUILD_DIR/ui

INCLUDEPATH += \
    $$PWD/include \
    $$PWD/include/ui \
    $$PWD/include/controllers \
    $$PWD/include/models \
    $$PWD/include/utils

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/ui/MainWindow.cpp \
    $$PWD/src/ui/UserInput.cpp \
    $$PWD/src/ui/HealthData.cpp \
    $$PWD/src/ui/HealthMetric.cpp \
    $$PWD/src/ui/Recommendations.cpp \
    $$PWD/src/ui/UserProfile.cpp \
    $$PWD/src/utils/DataProcessingManager.cpp \
    $$PWD/src/utils/DatabaseManager.cpp \
    $$PWD/src/utils/HealthMetricCalculator.cpp \
    $$PWD/src/models/BatteryModel.cpp \
    $$PWD/src/models/HealthDataModel.cpp \
    $$PWD/src/models/UserModel.cpp \
    $$PWD/src/controllers/HealthDataController.cpp \
    $$PWD/src/controllers/HealthMetricController.cpp \
    $$PWD/src/controllers/PowerManagementController.cpp \
    $$PWD/src/controllers/RecommendationsController.cpp \
    $$PWD/src/controllers/UserInputController.cpp \
    $$PWD/src/controllers/UserProfileController.cpp

HEADERS += \
    $$PWD/include/ui/MainWindow.h \
    $$PWD/include/ui/UserInput.h \
    $$PWD/include/ui/HealthData.h \
    $$PWD/include/ui/HealthMetric.h \
    $$PWD/include/ui/Recommendations.h \
    $$PWD/include/ui/UserProfile.h \
    $$PWD/include/utils/DataProcessingManager.h \
    $$PWD/include/utils/DatabaseManager.h \
    $$PWD/include/utils/HealthMetricCalculator.h \
    $$PWD/include/models/BatteryModel.h \
    $$PWD/include/models/HealthDataModel.h \
    $$PWD/include/models/UserModel.h \
    $$PWD/include/controllers/HealthDataController.h \
    $$PWD/include/controllers/HealthMetricController.h \
    $$PWD/include/controllers/PowerManagementController.h \
    $$PWD/include/controllers/RecommendationsController.h \
    $$PWD/include/controllers/UserInputController.h \
    $$PWD/include/controllers/UserProfileController.h

RESOURCES += \
    $$PWD/resources/dbData.qrc \
    $$PWD/resources/dbSchema.qrc

QMAKE_CXXFLAGS += -Wall -Wextra
CONFIG += object_with_source

OBJECTS_DIR = $$BUILD_DIR/obj
MOC_DIR = $$BUILD_DIR/moc

