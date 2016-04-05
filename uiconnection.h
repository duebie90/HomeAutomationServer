#ifndef UICONNECTION_H
#define UICONNECTION_H

#include <QObject>
#include <QtNetwork>
#include <iostream>
using namespace std;

class UiConnection: public QObject
{
    Q_OBJECT
public:
    UiConnection(QTcpSocket* socket, QString alias);

private slots:
    void slotReceivedData();
    void slotDisconnected();
private:    
    QTcpSocket* clientSocket;
    QString alias;

};

#endif // UICONNECTION_H
