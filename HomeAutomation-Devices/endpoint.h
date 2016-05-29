#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <QObject>
#include <QtNetwork>
#include <iostream>
#include <QTime>
#include <QDate>
#include <datareceiver.h>
#include <datatransmitter.h>
#include <ScheduleEvent.h>


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
    QList<ScheduleEvent*> getScheduledEvents();
    enum EndpointType {
        switchbox,
        temperatureSensor,
        lightSwitch
    };

public slots:
    //called by SchedulingService
    void slotPerformEvent(ScheduleEvent* event);
private slots:    
    void slotDisconnected();
    //from hardware endpoint
    void slotReceivedState(QString MAC, bool state);    
    //from ui-controller
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
    QList<ScheduleEvent*> schedulesEvents;
};

#endif // ENDPOINT_H
