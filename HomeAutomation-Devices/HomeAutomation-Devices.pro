#-------------------------------------------------
#
# Project created by QtCreator 2016-05-23T11:53:07
#
#-------------------------------------------------

QT       -= gui
QT       += core network
TARGET = HomeAutomation-Devices
TEMPLATE = lib
CONFIG += staticlib

include(HomeAutomation-Devices.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    HomeAutomation-Devices.pri
