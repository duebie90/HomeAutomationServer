#include "uiconnection.h"
#include <QtNetwork>


UiConnection::UiConnection(QTcpSocket* socket, QString alias, QObject* parent):
    QObject(parent)
{
    cout<<"UI Connection Object created with alias "<<alias.toStdString()<<".\n";
    this->clientSocket = socket;
    this->alias = alias;
    this->dataReceiver =  new UiDataReceiver();
    this->dataTransmitter = new UiDataTransmitter(socket);
    this->connected = true;
    connect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    //connections from data receiver
    connect(dataReceiver, SIGNAL(signalReceivedUiEndpointStateRequest(QString,bool)),
            this, SIGNAL(signalReceivedUiEndpointStateRequest(QString,bool)));
    connect(dataReceiver, SIGNAL(signalReceivedEndpointSchedule(QString,ScheduleEvent*)),
            this, SIGNAL(signalReceivedEndpointSchedule(QString,ScheduleEvent*)));
    connect(dataReceiver, SIGNAL(signalDeleteSchedule(QString, int)), this, SIGNAL(signalDeleteSchedule(QString,int)));
    connect(dataReceiver, SIGNAL(signalReceivedAutoRequest(QString,bool)), this, SIGNAL(signalReceivedAutoRequest(QString,bool)));
    connect(dataReceiver, SIGNAL(signalResetServer()), this, SIGNAL(signalResetServer()));
}
void UiConnection::sendMessage(QByteArray message){
    this->clientSocket->write(message, message.length());
}

void UiConnection::sendUpdate(QList<Endpoint *> endpoints)
{
    //send endpoint states list to ui
    sendEndpointStatesUpdate(endpoints);
    this->endpoints = endpoints;
    QTimer::singleShot(10, this, SLOT(slotPrepareEndpointSchedulesUpdate()) );
    //slotPrepareEndpointSchedulesUpdate();
}

void UiConnection::slotReceivedUiEndpointStateRequest(QString MAC, bool state) {


}

void UiConnection::slotPrepareEndpointSchedulesUpdate()
{
    static int i = 0;
    if(this->endpoints.length() > i) {
        Endpoint* endpoint = this->endpoints.at(i);
        sendEndpointSchedulesUpdate(endpoint->getMAC(), endpoint->getScheduledEvents().values());
        //recurse for next endpoint
        i++;
    }
    if(this->endpoints.length() > i) {
        QTimer::singleShot(10, this, SLOT(slotPrepareEndpointSchedulesUpdate()) );
    } else {
        i=0;
    }
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
        payload.append(PDU_DELIMITER);
        bool autoControlled = endpoint->isAutoControlled();
        payload.append(autoControlled ? "1": "0");
    }
    this->dataTransmitter->sendMessage(MESSAGETYPE_ENDPOINTS_STATES_LIST, payload);
}

void UiConnection::sendEndpointSchedulesUpdate(QString mac, QList<ScheduleEvent *> schedules)
{
    quint8 schedulesCount = schedules.length();
    QByteArray payload;
    QDataStream out(&payload, QIODevice::ReadWrite);
    out<<mac;
    out<<schedulesCount;
    foreach(ScheduleEvent* scheduleEvent, schedules) {
        out<<scheduleEvent;
    }
    this->dataTransmitter->sendMessage(MESSAGETYPE_ENDPOINTS_SCHEDULES_LIST, payload);
}

QHostAddress UiConnection::getIp()
{
    return this->clientSocket->peerAddress();
}

void UiConnection::slotDisconnected() {
    cout<<"Client Disconnected\n";
    this->connected = false;
    emit signalDisconnected();
}

