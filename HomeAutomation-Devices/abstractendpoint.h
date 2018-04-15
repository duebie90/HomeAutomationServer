#ifndef ABSTRACTENDPOINT_H
#define ABSTRACTENDPOINT_H

#include <QObject>
#include <QtNetwork>
#include <iostream>

class AbstractEndpoint : public QObject
{
    Q_OBJECT

public:
    enum EndpointType{
        SwitchboxEndpointType,
        HeatingEndpointType
    };
    AbstractEndpoint(QString alias="", QString type="", QString MAC="", QObject* parent=0);
    AbstractEndpoint();
    void copyEndpoint(AbstractEndpoint* otherEndpoint);
    virtual bool isConnected();
    virtual void setConnected(bool connected);
    QString getAlias();
    QString getMAC();
    virtual QString getType();
    virtual bool getState();
    virtual void setState(bool);

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
public slots:

private slots:

private:
    QString alias;
    QString type;
    QString MAC;
    bool connected;
    bool state;
    virtual void serialize(QDataStream &ds) = 0;
    virtual void unserialize(QDataStream &ds) = 0;
};

#endif // ABSTRACTENDPOINT_H
