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
    void slotReceivedData();
signals:
    void signalClientConnected(QTcpSocket *client);
    //Signal forwarded from DataReceiver to HomeAutomationController
    void signalReceivedEndpointIdent(QTcpSocket* socket, QString alias, QString type, QString MAC);
    void signalReceivedUiIdent(QTcpSocket* socket, QString alias, QString pass, QString MAC);

private:
    int processProtocollHeader(QTcpSocket* socket, QByteArray data);
    void processMessage(QTcpSocket* socket, MessageType type, QByteArray payload);
    QTcpServer *tcpServer;
    QNetworkSession *session;
    QList<QTcpSocket*> clientSockets;
    QString serverAddress;
    qint16 serverPort;
    //Recently connected clients:
    //We wait for them to send their identification before they are visualized
    QList<QTcpSocket*> clientsPendingIdentification;


};

#endif // TCPSERVER_H
