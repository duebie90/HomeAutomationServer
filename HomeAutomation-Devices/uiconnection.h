#ifndef UICONNECTION_H
#define UICONNECTION_H

#include <QObject>
#include <QtNetwork>
#include <iostream>
#include <UiDataReceiver.h>
#include <UiDataTransmitter.h>
#include <endpoint.h>

using namespace std;

class UiConnection: public QObject
{
    Q_OBJECT
public:
    UiConnection(QTcpSocket* socket, QString alias, QObject* parent=0);
    void sendMessage(QByteArray message);
    void sendUpdate(QList<AbstractEndpoint*> endpoints);
    void sendEndpointSchedulesUpdate(QString mac, QList<ScheduleEvent*> schedules);
    QHostAddress getIp();
    void ackIdentification();
private slots:  
    void slotDisconnected();
    void slotReceivedUiEndpointStateRequest(QString MAC, bool state);
    void slotReceivedEndpointSchedule(QString mac, ScheduleEvent* event);
    void slotReceivedAutoRequest(QString mac, bool autoControlled);
    void slotPrepareEndpointSchedulesUpdate();

signals:
    void signalResetServer();
    void signalDisconnected();
    void signalDeleteEndpoint(QString mac);
    void signalDeleteSchedule(QString mac, int scheduleId);
private:    
    void sendEndpointsUpdate(QList<AbstractEndpoint*> endpoints);

    QTcpSocket* clientSocket;
    UiDataReceiver* dataReceiver;
    UiDataTransmitter* dataTransmitter;
    QString alias;
    bool connected;
    QList<AbstractEndpoint*> endpoints;
};

#endif // UICONNECTION_H
