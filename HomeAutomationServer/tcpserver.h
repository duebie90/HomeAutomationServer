#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QNetworkSession>
#include <UiDataReceiver.h>
#include <UiDataTransmitter.h>

class TcpServer: public QObject
{
    Q_OBJECT

public:
    TcpServer(QString address="localhost", int port=3000, QObject *parent=0);
    ~TcpServer();
    void resetClientsPendingIdentification();
    //remove client from clientsPendingIdentification
    void clientIdentified(QTcpSocket* client);
private slots:
    void slotNetworkSessionOpened();
    void slotClientConnected();
    void slotAcceptError();       
signals:
    void signalClientConnected(QTcpSocket *client);
    //Signal forwarded from DataReceiver to HomeAutomationController
    void signalReceivedEndpointIdent(QTcpSocket* socket, QString alias, QString type, QString MAC);
    void signalReceivedUiIdent(QTcpSocket* socket, QString alias, QString pass, QString MAC);

private:
    QTcpServer *tcpServer;
    QNetworkSession *session;
    QList<QTcpSocket*> clientSockets;
    QString serverAddress;
    qint16 serverPort;
    //Recently connected clients:
    //We wait for them to send their identification before they are visualized
    QList<QTcpSocket*> clientsPendingIdentification;
    //Received data from unidentified clients
    DataReceiver* dataReceiver;
    //is mainly used to create server identification message
    DataTransmitter* dataTransmitter;

};

#endif // TCPSERVER_H
