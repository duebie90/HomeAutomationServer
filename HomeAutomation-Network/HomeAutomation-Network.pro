#-------------------------------------------------
#
# Project created by QtCreator 2016-05-23T11:58:52
#
#-------------------------------------------------

QT       += network websockets sql

QT       -= gui

TARGET = HomeAutomation-Network
TEMPLATE = lib
CONFIG += staticlib

include(HomeAutomation-Network.pri)

INCLUDEPATH += ../HomeAutomation-Devices

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    HomeAutomation-Network.pri
