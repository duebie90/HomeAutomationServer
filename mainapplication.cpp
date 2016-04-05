#include "mainapplication.h"
#include <QDebug>
#include <inputcontroller.h>


MainApplication::MainApplication(QObject *parent) : QObject(parent)
{
    app = QCoreApplication::instance();
}

MainApplication::~MainApplication() {
    delete(app);
}

void MainApplication::run() {
    qDebug()<<__FUNCTION__;
    cout<<"Home Automation Server \n";

    homeAutomationController = new HomeAutomationController();
    //QThread* homeAutomationControllerThread = new QThread();
    //homeAutomationController->moveToThread(homeAutomationControllerThread);

    //read input
    /*string input;
    do {
        cin>>input;
    } while(input != "q" && input != "quit" && input != "exit" );
    //eventually quit
    quit();*/
}

void MainApplication::slotAboutToQuit() {
    qDebug()<<__FUNCTION__;
    delete(homeAutomationController);

    //called by app
    //ToDo clean up and stop all thread
}

void MainApplication::quit() {
    //qDebug()<<__FUNCTION__;
    //possibly clean up
    //goes to app
    //emit signalFinished();
}
void MainApplication::slotQuit() {
    qDebug()<<__FUNCTION__;
    //possibly clean up
    //goes to app
    emit signalFinished();
}


