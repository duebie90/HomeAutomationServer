#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include <QObject>
#include <QCoreApplication>
#include <iostream>
using namespace std;

class InputController : public QObject
{
    Q_OBJECT
public:
    explicit InputController(QThread* icThread, QObject *parent = 0);

signals:
    void signalQuit();
public slots:
    void slotProcessInput();
private:
    QThread* inputControllerThread;

};

#endif // INPUTCONTROLLER_H
