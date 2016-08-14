#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T12:30:49
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = HomeAutomation-Services
TEMPLATE = lib
CONFIG += staticlib

include(HomeAutomation-Services.pri)

INCLUDEPATH += ../HomeAutomation-Devices \
                ../HomeAutomation-Network \
                ../HomeAutomationServer

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    ../HomeAutomationServer/HomeAutomation-Services.pri \
    HomeAutomation-Services.pri
