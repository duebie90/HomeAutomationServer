#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QNetworkSession>


class TcpServer: public QObject
{
    Q_OBJECT

public:
    TcpServer(QString address="localhost", int port=3000, QObject *parent=0);
    ~TcpServer();
private slots:
    void slotNetworkSessionOpened();
    void slotClientConnected();
    void slotAcceptError();   
signals:
    void signalClientConnected(QTcpSocket *client);


private:
    QTcpServer *tcpServer;
    QNetworkSession *session;
    QList<QTcpSocket*> clientSockets;
    QString serverAddress;
    qint16 serverPort;

};

#endif // TCPSERVER_H
