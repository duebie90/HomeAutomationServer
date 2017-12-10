#ifndef HOMEAUTOMATIONCONTROLLER_H
#define HOMEAUTOMATIONCONTROLLER_H

#include <QObject>
#include <endpoint.h>
#include <uiconnection.h>
#include <QtNetwork>
#include <tcpserver.h>
#include <UiDataReceiver.h>
#include <../HomeAutomation-Services/PersistanceService.h>
#include <SchedulingService.h>
#include <websocketserver.h>

class HomeAutomationController: public QObject
{
    Q_OBJECT
public:
    static HomeAutomationController *getInstance();
    void initiate();
    void deInitialize();
signals:
    void signalUpdateUis();
    void signalUpdateWss();
private slots:
    void slotUpdateUis();
    void slotProcessMessageNewUi(QTcpSocket* socket, QString alias, QString pass, QString MAC);
    void slotProcessMessageNewEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC);

    void slotForwardDeleteEndpoint(QString mac);
    void slotForwardEndpointDeleteSchedule(QString mac, int id);
    void slotResetServer();
    void slotDeleteEndpoint(QString MAC);
    void slotUiDisconnected();
private:
    //This is a singlton class
    HomeAutomationController(QObject* parent=0);
    ~HomeAutomationController();
    static HomeAutomationController *_instance;

    void addUiConnection(QTcpSocket* socket, QString alias);
    void addEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC);

    TcpServer* tcpServer;
    //Clients which are identified as Endpoints
    //have to be confirmed by user (via UI)
    QList<QTcpSocket*> endpointsPendingConfirmation;
    //Instances of Endpoint or uiConnection are created
    //to handle identified client-connections
    //QList<Endpoint*> endpoints;
    QList<UiConnection*> uiConnections;
    //Map necessary for client recognition on reconnect

    //QMap<QString, Endpoint*> mapMacToEndpoint;
    //QUuid hacUuid;
    QString pwd;
    QTimer* uiUpdateTimer;
    QSettings settings;

    //Services
    //ToDo create Services Handler
    PersistanceService* ps;
    SchedulingService*  ss;
    WsServer* wss;
    UiConnection* ui2Update;

};

#endif // HOMEAUTOMATIONCONTROLLER_H
