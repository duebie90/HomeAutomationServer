QT += core network
#QT -= gui

TARGET = HomeAutomationServer
CONFIG += console
CONFIG -= app_bundle

#TEMPLATE = app

SOURCES += main.cpp \
    endpoint.cpp \
    uiconnection.cpp \
    tcpserver.cpp \
    homeautomationcontroller.cpp \
    mainapplication.cpp \
    inputcontroller.cpp

HEADERS += \
    endpoint.h \
    uiconnection.h \
    tcpserver.h \
    homeautomationcontroller.h \
    mainapplication.h \
    inputcontroller.h

