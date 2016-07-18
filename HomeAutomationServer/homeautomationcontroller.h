#ifndef HOMEAUTOMATIONCONTROLLER_H
#define HOMEAUTOMATIONCONTROLLER_H

#include <QObject>
#include <endpoint.h>
#include <uiconnection.h>
#include <QtNetwork>
#include <tcpserver.h>
#include <UiDataReceiver.h>
#include <PersistanceService.h>
#include <SchedulingService.h>


class HomeAutomationController: public QObject
{
    Q_OBJECT
public:
    HomeAutomationController(QObject* parent=0);
    ~HomeAutomationController();    
private slots:
    void slotUpdateUis();
    void slotProcessMessageNewUi(QTcpSocket* socket, QString alias, QString pass, QString MAC);
    void slotProcessMessageNewEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC);
    //forwarding of a state change Request to endpoints (called by UiConnection)
    void slotForwardStateChangeRequest(QString MAC, bool state);
    void slotForwardEndpointSchedule(QString MAC, ScheduleEvent* event);
    void slotForwardEndpointAutoRequest(QString MAC, bool autoMode);
    void slotResetServer();
    void slotDeleteEndpoint(QString MAC);
    void slotUiDisconnected();
private:
    void addUiConnection(QTcpSocket* socket, QString alias);
    void addEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC);

    TcpServer* tcpServer;
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
    QTimer* uiUpdateTimer;
    QSettings settings;

    //Services
    //ToDo create Services Handler
    PersistanceService* ps;
    SchedulingService*  ss;

    UiConnection* ui2Update;

};

#endif // HOMEAUTOMATIONCONTROLLER_H
