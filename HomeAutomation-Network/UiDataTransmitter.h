#ifndef UiDataTransmitter_H
#define UiDataTransmitter_H

#include <QObject>
#include <messagetype.h>
#include <QtNetwork>
#include <iostream>

class UiDataTransmitter: public QObject
{
    Q_OBJECT
public:
    UiDataTransmitter(QTcpSocket* client=0, QObject* parent=0);
    void updateSocket(QTcpSocket* socket);
    int sendInfoMessage();
    int sendMessage(MessageType type, QByteArray payload);
    QByteArray prepareMessage(MessageType type, QByteArray payload);
public slots:
private slots:

signals:

private:   
   QTcpSocket* client;

};

#endif // UiDataTransmitter_H
