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
#include <abstractendpoint.h>

using namespace std;
class Endpoint: public AbstractEndpoint
{
Q_OBJECT
public:
    Endpoint(QTcpSocket* socket=nullptr, QString alias="", EndpointTypes type=ENDPOINT_TYPE_SWITCHBOX, QString MAC="", QObject* parent=nullptr);
    ~Endpoint();
    void updateSocket(QTcpSocket* newSocket);
    bool getRequestedState();
    void setAuto(bool autoControlled);
    bool isAutoControlled();    
    void requestState(bool state);
    void sendMessage(MessageType type, QByteArray message);
    QMap<int, ScheduleEvent*> getScheduledEvents();
    //is used to either update or add a new event
    void addScheduleEvent(ScheduleEvent* event);
    void updateScheduleEvent(ScheduleEvent* event);
    void removeSchedule(int id);
    bool isStateChangePending();

    //friend QDataStream &ScheduleEvent::operator<<(QDataStream &ds, AbstractEndpoint *obj)
    //serialize to send
    friend QDataStream &operator<<(QDataStream &ds, Endpoint *obj)
    {
       //write serialized class on data stream
       obj->serialize(ds);
       return ds;
    }

    //deserialize from telegram
    //friend QDataStream &ScheduleEvent::operator>>(QDataStream &in,AbstractEndpoint *sEvent)
    friend QDataStream &operator>>(QDataStream &in,Endpoint *obj)
    {
       //qint16 id;
       //in>>id;
       //set corresponding attributes in current object
       obj->unserialize(in);
       return in;
    }

signals:
    void signalSchedulesChanged();
    void signalStateChanged();
    void signalConnectedChanged();
public slots:
    //called by SchedulingService
    void slotPerformEvent(ScheduleEvent* event);    
private slots:        
    //from hardware endpoint
    void slotReceivedState(QString MAC, bool state);    
    //from ui-controller
    void slotStateRequested(bool state);
private:

    void serialize(QDataStream &ds);
    void unserialize(QDataStream &ds);
    //state which was ACK by endpoint
    bool state;
    //state which was requested: manually or automatically
    bool requestedState;

    bool stateChangePending;

    bool autoControlled;
    //state which was there if auto-controlled
    bool autoControlledState;

    QMap<int, ScheduleEvent*> scheduleEvents;
};

#endif // ENDPOINT_H
