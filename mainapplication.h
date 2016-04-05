#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QObject>
#include <QCoreApplication>

#include <tcpserver.h>
#include <homeautomationcontroller.h>
#include <uiconnection.h>

#include <iostream>
using namespace std;

class MainApplication : public QObject
{
    Q_OBJECT
public:
    explicit MainApplication(QObject *parent = 0);
    ~MainApplication();
private:
    //is called to terminate the programm
    void quit();
    QCoreApplication* app;
    HomeAutomationController* homeAutomationController;

signals:
    //is emitted sent to the application to terminate it
    void signalFinished();
public slots:
    void slotQuit();
    void run();
    void slotAboutToQuit();
};

#endif // MAINAPPLICATION_H
