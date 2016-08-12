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
    void sendUpdate(QList<Endpoint*> endpoints);
    void sendEndpointSchedulesUpdate(QString mac, QList<ScheduleEvent*> schedules);
    QHostAddress getIp();
private slots:  
    void slotDisconnected();
    void slotReceivedUiEndpointStateRequest(QString MAC, bool state);
    void slotPrepareEndpointSchedulesUpdate();
signals:
    void signalReceivedUiEndpointStateRequest(QString MAC, bool state);
    void signalReceivedEndpointSchedule(QString mac, ScheduleEvent* event);
    void signalReceivedAutoRequest(QString mac, bool autoControlled);
    void signalResetServer();
    void signalDisconnected();
    void signalDeleteEndpoint(QString mac);
    void signalDeleteSchedule(QString mac, int scheduleId);
private:    
    void sendEndpointStatesUpdate(QList<Endpoint*> endpoints);

    QTcpSocket* clientSocket;
    UiDataReceiver* dataReceiver;
    UiDataTransmitter* dataTransmitter;
    QString alias;
    bool connected;
    QList<Endpoint*> endpoints;
};

#endif // UICONNECTION_H
