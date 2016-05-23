TEMPLATE = subdirs

SUBDIRS +=  HomeAutomation-Network \
            HomeAutomation-Devices \
            HomeAutomation-Services \
            HomeAutomationServer

HomeAutomation-Services.subdir = HomeAutomation-Services
HomeAutomationServer           = HomeAutomationServer

CONFIG += ordered

HomeAutomationServer.depends += HomeAutomation-Network \
                                HomeAutomation-Devices \
                                HomeAutomation-Services

PRE_TARGETDEPS += HomeAutomation-Network/ HomeAutomation-Network \
                  HomeAutomation-Devices/ HomeAutomation-Devices \
                  HomeAutomation-Services/ HomeAutomation-Devices

