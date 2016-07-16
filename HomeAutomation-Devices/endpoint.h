#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <QObject>
#include <QtNetwork>
#include <iostream>
#include <QTime>
#include <QDate>
#include <EndpointDataReceiver.h>
#include <EndpointDataTransmitter.h>
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
    void setAuto(bool autoControlled);
    bool isAutoControlled();
    bool ackIdentification();
    void requestState(bool state);
    void sendMessage(MessageType type, QByteArray message);
    QMap<int, ScheduleEvent*> getScheduledEvents();
    //is used to either update or add a new event
    void updateScheduleEvent(ScheduleEvent* event);
    enum EndpointType {
        //those are example
        switchbox,
        temperatureSensor,
        lightSwitch
    };
signals:
    void signalSchedulesChanged();
public slots:
    //called by SchedulingService
    void slotPerformEvent(ScheduleEvent* event);
    void slotReceivedIdentMessage(QTcpSocket* socket, QString alias, QString type, QString MAC);
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
    EndpointDataReceiver* dataReceiver;
    EndpointDataTransmitter* dataTransmitter;
    bool connected;
    //state which was ACK by endpoint
    bool state;
    //state which was requested: manually or automatically
    bool requestedState;
    bool autoControlled;
    //state which was there if auto-controlled
    bool autoControlledState;


    QMap<int, ScheduleEvent*> scheduleEvents;
};

#endif // ENDPOINT_H
