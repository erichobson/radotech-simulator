TEMPLATE = app
TARGET = RaDoTech

QT += core gui widgets sql

CONFIG += c++17
CONFIG -= app_bundle

CONFIG += debug
DEFINES += QT_DEBUG

INCLUDEPATH += \
    $$PWD/include \
    $$PWD/include/tests \
    $$PWD/include/ui \
    $$PWD/include/controllers \
    $$PWD/include/models \
    $$PWD/include/utils \

RESOURCES += resources/resources.qrc

include($$PWD/build/qmake/sources.pri)
include($$PWD/build/qmake/headers.pri)
