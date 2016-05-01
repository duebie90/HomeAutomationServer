#ifndef DATATRANSMITTER_H
#define DATATRANSMITTER_H

#include <QObject>
#include <messagetype.h>
#include <QtNetwork>

class DataTransmitter: public QObject
{
    Q_OBJECT
public:
    DataTransmitter(QTcpSocket* client=0, QObject* parent=0);
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

#endif // DATATRANSMITTER_H
