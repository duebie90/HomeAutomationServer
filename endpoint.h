#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <QObject>
#include <QtNetwork>
#include <iostream>
#include <datareceiver.h>
#include <datatransmitter.h>

using namespace std;
class Endpoint: public QObject
{
    Q_OBJECT
public:
    Endpoint(QTcpSocket* socket, QString alias, QString type, QString MAC="", QObject* parent=0);
    ~Endpoint();
    void updateSocket(QTcpSocket* newSocket);
    bool isConnected();
    void setConnected(bool connected);
    QString getAlias();
    QString getType();
    QString getMAC();
    void setState(bool state);
    bool getState();
    void requestState(bool state);
    void sendMessage(MessageType type, QByteArray message);
    enum EndpointType {
        switchbox,
        temperatureSensor,
        lightSwitch
    };

private slots:    
    void slotDisconnected();
    void slotReceivedState(QString MAC, bool state);    
    void slotStateRequested(bool state);
private:
    void receivedData();
    QString alias;
    QString type;
    QString MAC;    
    QTcpSocket* clientSocket;
    DataReceiver* dataReceiver;
    DataTransmitter* dataTransmitter;
    bool connected;
    bool state;
};

#endif // ENDPOINT_H
