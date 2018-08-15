#ifndef ABSTRACTENDPOINT_H
#define ABSTRACTENDPOINT_H

#include <QObject>
#include <QtNetwork>
#include <iostream>
#include <EndpointDataReceiver.h>
#include <EndpointDataTransmitter.h>
#include <endpointtypes.h>

//Timeout for next keep alive messages
const int KEEP_ALIVE_TIMEOUT_MS = 5000;
using namespace  std;

class AbstractEndpoint : public QObject
{
Q_OBJECT
public:
    AbstractEndpoint(QTcpSocket* socket=nullptr, QString alias="", EndpointTypes type=ENDPOINT_TYPE_SWITCHBOX, QString MAC="", QObject* parent=nullptr);
    AbstractEndpoint();
    bool isConnected();
    void setConnected(bool connected);
    QString getAlias();
    void setAlias(QString alias);    
    QString getMAC();
    void setMAC(QString MAC);
    EndpointTypes getType();
    virtual bool getState();
    virtual void setState(bool);
    virtual void updateSocket(QTcpSocket* newSocket);
    bool ackIdentification();
    void sendMessage(MessageType type, QByteArray message);
    //friend QDataStream &ScheduleEvent::operator<<(QDataStream &ds, AbstractEndpoint *obj)
    //serialize to send
    friend QDataStream &operator<<(QDataStream &ds, AbstractEndpoint *obj)
    {
       //write serialized class on data stream
       obj->serialize(ds);
       return ds;
    }

    //deserialize from telegram
    //friend QDataStream &ScheduleEvent::operator>>(QDataStream &in,AbstractEndpoint *sEvent)
    friend QDataStream &operator>>(QDataStream &in,AbstractEndpoint *obj)
    {
       //qint16 id;
       //in>>id;
       //set corresponding attributes in current object
       obj->unserialize(in);
       return in;
    }

signals:
    void signalUpdateEndpoint();
    void signalSchedulesChanged();
    void signalStateChanged();
    void signalConnectedChanged();
public slots:
    void slotReceivedIdentMessage(QTcpSocket* socket, QString alias, QString type, QString MAC);
protected slots:
    void slotDisconnected();
    void slotSocketError(QAbstractSocket::SocketError socketError);
    void slotResetTimeout();
    void slotKeepAliveTimeout();
protected:
    QString alias;
    EndpointTypes type;
    QString MAC;
    QTcpSocket* clientSocket;
    EndpointDataReceiver* dataReceiver;
    EndpointDataTransmitter* dataTransmitter;
    bool connected;
    bool state;
private:
    void receivedData();
    //This timer is started if after each received state (equals keep Alive heartbeat)
    //if it triggers this endpoint eappears to be disconnected and the socket is closed
    QTimer* keepAliveTimeoutTimer;
    virtual void serialize(QDataStream &ds) = 0;
    virtual void unserialize(QDataStream &ds) = 0;
};

#endif // ABSTRACTENDPOINT_H
