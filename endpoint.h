#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <QObject>
#include <QtNetwork>
#include <iostream>
using namespace std;

class Endpoint: public QObject
{
    Q_OBJECT
public:
    Endpoint(QTcpSocket* socket, QString alias, QString type);
    void updateSocket(QTcpSocket* newSocket);
    bool isConnected();
    QString getAlias();
    QString getType();
    QString getMAC();
    enum EndpointType {
        switchbox,
        temperatureSensor,
        lightSwitch
    };

private slots:
    void slotReceivedData();
    void slotDisconnected();
private:
    void receivedData();
    QString alias;
    QString type;
    QString MAC;
    QTcpSocket* clientSocket;
    bool connected;
};

#endif // ENDPOINT_H
