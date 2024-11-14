# @file RaDoTech.pro

QT += core gui widgets

CONFIG += c++17
CONFIG -= app_bundle

TEMPLATE = app
TARGET = RaDoTech

BUILD_DIR = $$PWD/build
OBJECTS_DIR = $${BUILD_DIR}/obj
MOC_DIR = $${BUILD_DIR}/moc
UI_DIR = $${BUILD_DIR}/ui

DESTDIR = $${BUILD_DIR}
QMAKE_POST_LINK += ln -sf $${BUILD_DIR}/$${TARGET}  $$PWD/$${TARGET}

INCLUDEPATH += include \
               ui

SOURCES += \
    src/.cpp \
    src/.cpp \
    ui/MainWindow.cpp \
    main.cpp

HEADERS += \
    include/.h \
    include/.h \
    include/.h \
    ui/MainWindow.h

QMAKE_CXXFLAGS += -Wall -Wextra
