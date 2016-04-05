#include "inputcontroller.h"
#include <QDebug>
#include <QThread>

InputController::InputController(QThread* icThread,QObject *parent) : QObject(parent)
{
    qDebug()<<__FUNCTION__;
    icThread = new QThread();
    this->moveToThread(icThread);
    QObject::connect(icThread, SIGNAL(started()), this, SLOT(slotProcessInput()));
    this->inputControllerThread = icThread;
    icThread->start();
}

void InputController::slotProcessInput() {
    //read input
    string input;
    do {
        cin>>input;
    } while(input != "q" && input != "quit" && input != "exit" );
    //eventually quit
    cout<<"Quittin Application\n";
    this->inputControllerThread->terminate();
    emit signalQuit();
}





