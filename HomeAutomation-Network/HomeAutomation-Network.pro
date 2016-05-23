#-------------------------------------------------
#
# Project created by QtCreator 2016-05-23T11:58:52
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = HomeAutomation-Network
TEMPLATE = lib
CONFIG += staticlib

include(HomeAutomation-Network.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    HomeAutomation-Network.pri
