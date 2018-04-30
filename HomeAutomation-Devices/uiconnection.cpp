#include "uiconnection.h"
#include <QtNetwork>

//#include <../HomeAutomation-Services/PersistanceService.h>

class PersistanceService;

#include <../HomeAutomation-Services/PersistanceService.h>

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
            this, SLOT(slotReceivedUiEndpointStateRequest(QString,bool)));
    connect(dataReceiver, SIGNAL(signalReceivedEndpointSchedule(QString,ScheduleEvent*)),
            this, SLOT(slotReceivedEndpointSchedule(QString,ScheduleEvent*)));
    connect(dataReceiver, SIGNAL(signalDeleteEndpoint(QString)),
            this, SIGNAL(signalDeleteEndpoint(QString)));
    connect(dataReceiver, SIGNAL(signalDeleteSchedule(QString, int)),
            this, SIGNAL(signalDeleteSchedule(QString,int)));
    connect(dataReceiver, SIGNAL(signalReceivedAutoRequest(QString,bool)),
            this, SLOT(slotReceivedAutoRequest(QString,bool)));
    connect(dataReceiver, SIGNAL(signalResetServer()), this, SIGNAL(signalResetServer()));
}
void UiConnection::sendMessage(QByteArray message){
    this->clientSocket->write(message, message.length());
}

void UiConnection::sendUpdate(QList<AbstractEndpoint *> endpoints)
{
    //send endpoint states list to ui
    //sendEndpointStatesUpdate(endpoints);
    sendEndpointsUpdate(endpoints);
    this->endpoints = endpoints;
    QTimer::singleShot(10, this, SLOT(slotPrepareEndpointSchedulesUpdate()) );
    //slotPrepareEndpointSchedulesUpdate();
}

void UiConnection::slotReceivedUiEndpointStateRequest(QString MAC, bool state) {
    PersistanceService* ps = PersistanceService::getInstance();
    Endpoint* endpoint = dynamic_cast<Endpoint*>(ps->getEndpointByMac(MAC));
    if (endpoint != NULL) {
        endpoint->requestState(state);
    }
}

void UiConnection::slotReceivedEndpointSchedule(QString mac, ScheduleEvent *event)
{
    PersistanceService* ps = PersistanceService::getInstance();
    Endpoint* endpoint = dynamic_cast<Endpoint*>(ps->getEndpointByMac(mac));
    if(endpoint != NULL) {
        endpoint->updateScheduleEvent(event);
    }
}

void UiConnection::slotReceivedAutoRequest(QString mac, bool autoMode)
{
    PersistanceService* ps = PersistanceService::getInstance();
    Endpoint* endpoint = dynamic_cast<Endpoint*>(ps->getEndpointByMac(mac));
    if(endpoint != NULL) {
        endpoint->setAuto(autoMode);
    }
}

void UiConnection::slotPrepareEndpointSchedulesUpdate()
{
    static int i = 0;
    if(this->endpoints.length() > i) {
        Endpoint* endpoint = dynamic_cast<Endpoint*>(this->endpoints.at(i));
        if(endpoint!= NULL){
            sendEndpointSchedulesUpdate(endpoint->getMAC(), endpoint->getScheduledEvents().values());
        }
        //recurse for next endpoint
        i++;
    }
    if(this->endpoints.length() > i) {
        QTimer::singleShot(10, this, SLOT(slotPrepareEndpointSchedulesUpdate()) );
    } else {
        i=0;
    }
}


void UiConnection::sendEndpointsUpdate(QList<AbstractEndpoint *> endpoints){
    quint8 endpointsCount = endpoints.length();
    QByteArray payload;
    QDataStream out(&payload, QIODevice::ReadWrite);
    QString endpointType = "SwitchBox";
    out<<endpointsCount;
    foreach(AbstractEndpoint* endpoint, endpoints ) {
        out<<endpointType;
        out<<endpoint;
    }
    this->dataTransmitter->sendMessage(MESSAGETYPE_ENDPOINTS_LIST, payload);
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

void UiConnection::ackIdentification()
{
    cout<<__FUNCTION__<<"\n";
    QByteArray payload;
    //empty payload
    this->dataTransmitter->sendMessage(MESSAGETYPE_UI_INFO_ACK, payload);
}

void UiConnection::slotDisconnected() {
    cout<<"Client Disconnected\n";
    this->connected = false;
    emit signalDisconnected();
}

