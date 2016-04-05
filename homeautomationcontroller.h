#ifndef HOMEAUTOMATIONCONTROLLER_H
#define HOMEAUTOMATIONCONTROLLER_H

#include <QObject>
#include <endpoint.h>
#include <uiconnection.h>
#include <QtNetwork>
#include <tcpserver.h>


class HomeAutomationController: public QObject
{
    Q_OBJECT
public:
    HomeAutomationController(QObject* parent=0);
    ~HomeAutomationController();
private slots:
    void slotClientConnected(QTcpSocket* clientSocket);
    void slotUnidentifiedClientMessage();
private:
    bool processMessageNewUi(QString message, QTcpSocket* socket);
    bool processMessageNewEndpoint(QString message, QTcpSocket* socket);
    void addUiConnection(QTcpSocket* socket, QString alias);
    void addEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC);

    TcpServer* tcpServer;
    //Recently connected clients:
    //We wait for them to send their identification before they are visualized
    QList<QTcpSocket*> clientsPendingIdentification;
    //Clients which are identified as Endpoints
    //have to be confirmed by user (via UI)
    QList<QTcpSocket*> endpointsPendingConfirmation;
    //Instances of Endpoint or uiConnection are created
    //to handle identified client-connections
    QList<Endpoint*> endpoints;
    QList<UiConnection*> uiConnections;
    //Map necessary for client recognition on reconnect

    QMap<QString, Endpoint*> mapMacToEndpoint;
    //QUuid hacUuid;
    QString pwd;
};

#endif // HOMEAUTOMATIONCONTROLLER_H
