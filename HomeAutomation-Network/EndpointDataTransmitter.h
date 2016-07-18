#ifndef EndpointDataTransmitter_H
#define EndpointDataTransmitter_H

#include <QObject>
#include <messagetype.h>
#include <QtNetwork>
#include <iostream>

class EndpointDataTransmitter: public QObject
{
    Q_OBJECT
public:
    EndpointDataTransmitter(QTcpSocket* client=0, QObject* parent=0);
    void updateSocket(QTcpSocket* socket);
    int sendStateRequest(QString mac, bool state);
    int sendMessage(MessageType type, QByteArray payload);
    QByteArray prepareMessage(MessageType type, QByteArray payload);
public slots:
private slots:

signals:

private:   
   QTcpSocket* client;

};

#endif // EndpointDataTransmitter_H
