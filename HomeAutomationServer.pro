QT += core network sql
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
    inputcontroller.cpp \
    datareceiver.cpp \
    datatransmitter.cpp \
    persistanceservice.cpp

HEADERS += \
    endpoint.h \
    uiconnection.h \
    tcpserver.h \
    homeautomationcontroller.h \
    mainapplication.h \
    inputcontroller.h \
    datareceiver.h \
    messagetype.h \
    datatransmitter.h \
    persistanceservice.h

RC_FILE =  app.rc


