QT += core network sql
QT -= gui

TARGET = HomeAutomationServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    homeautomationcontroller.cpp \
    inputcontroller.cpp \
    mainapplication.cpp \
    tcpserver.cpp

INCLUDEPATH +=  ../HomeAutomation-Services \
                ../HomeAutomation-Devices \
                ../HomeAutomation-Network

HEADERS +=  homeautomationcontroller.h \
            inputcontroller.h \
            mainapplication.h \
            tcpserver.h

RC_FILE =  app.rc




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../HomeAutomation-Services/release/ -lHomeAutomation-Services
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../HomeAutomation-Services/debug/ -lHomeAutomation-Services
else:unix: LIBS += -L$$OUT_PWD/../HomeAutomation-Services/ -lHomeAutomation-Services

INCLUDEPATH += $$PWD/../HomeAutomation-Services
DEPENDPATH += $$PWD/../HomeAutomation-Services

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Services/release/libHomeAutomation-Services.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Services/debug/libHomeAutomation-Services.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Services/release/HomeAutomation-Services.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Services/debug/HomeAutomation-Services.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Services/libHomeAutomation-Services.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../HomeAutomation-Devices/release/ -lHomeAutomation-Devices
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../HomeAutomation-Devices/debug/ -lHomeAutomation-Devices
else:unix: LIBS += -L$$OUT_PWD/../HomeAutomation-Devices/ -lHomeAutomation-Devices

INCLUDEPATH += $$PWD/../HomeAutomation-Devices
DEPENDPATH += $$PWD/../HomeAutomation-Devices

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Devices/release/libHomeAutomation-Devices.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Devices/debug/libHomeAutomation-Devices.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Devices/release/HomeAutomation-Devices.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Devices/debug/HomeAutomation-Devices.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Devices/libHomeAutomation-Devices.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../HomeAutomation-Network/release/ -lHomeAutomation-Network
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../HomeAutomation-Network/debug/ -lHomeAutomation-Network
else:unix: LIBS += -L$$OUT_PWD/../HomeAutomation-Network/ -lHomeAutomation-Network

INCLUDEPATH += $$PWD/../HomeAutomation-Network
DEPENDPATH += $$PWD/../HomeAutomation-Network

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Network/release/libHomeAutomation-Network.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Network/debug/libHomeAutomation-Network.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Network/release/HomeAutomation-Network.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Network/debug/HomeAutomation-Network.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../HomeAutomation-Network/libHomeAutomation-Network.a
