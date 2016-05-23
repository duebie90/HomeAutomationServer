#include <QCoreApplication>
#include <iostream>
#include <mainapplication.h>
#include <inputcontroller.h>
#include <QTimer>

using namespace std;


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("HomeAutomationServer");
    QCoreApplication::setOrganizationName("Fh-Kiel");

    //Start MainApplication
    MainApplication* mainApplication = new MainApplication();
    //
    QObject::connect(mainApplication, SIGNAL(signalFinished()), &app, SLOT(quit()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), mainApplication, SLOT(slotAboutToQuit()));
    QTimer::singleShot(10, mainApplication, SLOT(run()));

    //create separate Thread to process user-input
    //e.g. quit commands
    QThread* icThread = new QThread();
    InputController* ic = new InputController(icThread);
    QObject::connect(ic, SIGNAL(signalQuit()), mainApplication, SLOT(slotQuit()));
    //ic->moveToThread(icThread);
    //QObject::connect(icThread, SIGNAL(started()), ic, SLOT(slotProcessInput()));
    //icThread->start();

    QObject::connect(mainApplication, SIGNAL(signalFinished()), icThread, SLOT(terminate()));


    return app.exec();
}

