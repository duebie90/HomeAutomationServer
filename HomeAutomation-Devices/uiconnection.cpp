#include "uiconnection.h"
#include <QtNetwork>


UiConnection::UiConnection(QTcpSocket* socket, QString alias, QObject* parent):
    QObject(parent)
{
    cout<<"UI Connection Object created with alias "<<alias.toStdString()<<".\n";
    this->clientSocket = socket;
    this->alias = alias;
    this->dataReceiver =  new DataReceiver();
    this->dataTransmitter = new DataTransmitter(socket);
    this->connected = true;
    connect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    //connections from data receiver
    connect(dataReceiver, SIGNAL(signalReceivedUiEndpointStateRequest(QString,bool)),
            this, SIGNAL(signalReceivedUiEndpointStateRequest(QString,bool)));
    connect(dataReceiver, SIGNAL(signalReceivedEndpointSchedule(QString,ScheduleEvent*)),
            this, SIGNAL(signalReceivedEndpointSchedule(QString,ScheduleEvent*)));


    connect(dataReceiver, SIGNAL(signalResetServer()), this, SIGNAL(signalResetServer()));
}
void UiConnection::sendMessage(QByteArray message){
    this->clientSocket->write(message, message.length());
}

void UiConnection::sendUpdate(QList<Endpoint *> endpoints)
{
    //send endpoint states list to ui
    sendEndpointStatesUpdate(endpoints);

    //foreach(Endpoint* endpoint, endpoints ) {
    //    sendEndpointSchedulesUpdate(endpoint->getMAC(), endpoint->getScheduledEvents().values());
    //}
}

void UiConnection::slotReceivedUiEndpointStateRequest(QString MAC, bool state) {


}

void UiConnection::sendEndpointStatesUpdate(QList<Endpoint *> endpoints)
{
    QByteArray payload;
    foreach(Endpoint* endpoint, endpoints ) {
        if(payload !="") {
            payload.append(PDU_DELIMITER);
        }
        payload.append(endpoint->getAlias());
        payload.append(PDU_DELIMITER);
        payload.append(endpoint->getMAC());
        payload.append(PDU_DELIMITER);
        payload.append(endpoint->getType());
        payload.append(PDU_DELIMITER);
        bool state = endpoint->getState();
        payload.append(state ? "1": "0");
        payload.append(PDU_DELIMITER);
        bool connected = endpoint->isConnected();
        payload.append(connected ? "1": "0");
    }
    this->dataTransmitter->sendMessage(MESSAGETYPE_ENDPOINTS_STATES_LIST, payload);
}

void UiConnection::sendEndpointSchedulesUpdate(QString mac, QList<ScheduleEvent *> schedules)
{
    quint8 schedulesCount = schedules.length();
    if (schedulesCount != 0) {
        QByteArray payload;
        QDataStream out(&payload, QIODevice::ReadWrite);
        out<<mac;
        out<<schedulesCount;
        foreach(ScheduleEvent* scheduleEvent, schedules) {
            out<<scheduleEvent;
        }
        this->dataTransmitter->sendMessage(MESSAGETYPE_ENDPOINTS_SCHEDULES_LIST, payload);
    }
}

void UiConnection::slotDisconnected() {
    cout<<"Client Disconnected\n";
    this->connected = false;
}

