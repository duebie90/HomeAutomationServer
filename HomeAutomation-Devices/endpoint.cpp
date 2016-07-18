#include "endpoint.h"

Endpoint::Endpoint(QTcpSocket* socket, QString alias, QString type, QString MAC, QObject* parent):
    QObject(parent)
{
    cout<<"Endpoint-Object created with alias "<<alias.toStdString()<<" and type "<<type.toStdString()<<".\n";
    this->clientSocket = socket;
    this->alias = alias;
    this->type = type;
    this->MAC = MAC;
    this->state = false;
    this->requestedState = false;
    this->dataReceiver = new EndpointDataReceiver();
    this->dataTransmitter = new EndpointDataTransmitter(socket);
    this->autoControlled = false;
    this->autoControlledState = false;
    if(clientSocket != NULL) {
        connect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
        connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    }

    connect(dataReceiver, SIGNAL(signalReceivedEndpointState(QString,bool)), this, SLOT(slotReceivedState(QString,bool)));
    connect(dataReceiver, SIGNAL(signalReceivedEndpointIdent(QTcpSocket*,QString,QString,QString)), this,
            SLOT(slotReceivedIdentMessage(QTcpSocket*,QString,QString,QString)));
    this->connected = true;



}

Endpoint::~Endpoint() {
    qDebug()<<__FUNCTION__;
    slotDisconnected();
    delete dataReceiver;
    delete dataTransmitter;
    if(clientSocket != NULL) {
        this->clientSocket->close();
    }
    delete clientSocket;
}

void Endpoint::slotReceivedState(QString MAC, bool state) {
    qDebug()<<__FUNCTION__<<"new State= "<<state;
    if(MAC == this->MAC) {
        setState(state);
    }
}

void Endpoint::slotStateRequested(bool state) {




}

void Endpoint::slotPerformEvent(ScheduleEvent *event)
{
    qDebug()<<__FUNCTION__<<" Alias: "<<getAlias()<<" EventType: "<<event->getType();
    ScheduleEvent::ScheduleEventType type = event->getType();
   if (this->autoControlled) {
    if (type == ScheduleEvent::EVENT_ON) {
        requestState(true);
        event->setPerformed();
    } else if(type == ScheduleEvent::EVENT_OFF) {
        requestState(false);
        event->setPerformed();
    }
   } else {
    //not auto controlled at the moment, save auto-state for later

   }

    //ToDo: implement more possible state changes
    //e.g. analog etc

    //If the event has no repetition, and was performed, dequeue
    if (!event->isPending()) {
        this->scheduleEvents.remove(event->getId());
    }
}

void Endpoint::slotReceivedIdentMessage(QTcpSocket *socket, QString alias, QString type, QString MAC)
{
    Q_UNUSED(socket)
    Q_UNUSED(alias)
    Q_UNUSED(type)
    if(QString::compare(MAC, this->getMAC(),Qt::CaseSensitive) == 0) {
        ackIdentification();
    }
}

void Endpoint::sendMessage(MessageType type, QByteArray message){
    this->dataTransmitter->sendMessage(type, message);
}

QMap<int, ScheduleEvent*> Endpoint::getScheduledEvents()
{
    return this->scheduleEvents;
}

void Endpoint::updateScheduleEvent(ScheduleEvent* event)
{
    if (this->scheduleEvents.contains(event->getId())) {
        ScheduleEvent* oldEvent = scheduleEvents.value(event->getId());
        //using stream operator to update all values
        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::ReadWrite);
        stream<<event;
        stream.device()->reset();
        stream>>oldEvent;
    } else if(event->getId() == scheduleEvents.size()){
        this->scheduleEvents.insert(event->getId(), event);
    } else if(event->getId() > scheduleEvents.size()){
        int id = scheduleEvents.size();
        event->setId(id);
        this->scheduleEvents.insert(id, event);
    } else {
        cout<<"Endpoint "<<getMAC().toStdString()<<" : Error inserting schedule event\n";
        cout<<"Id"<<event->getId()<<" is invalid";
    }
    emit signalSchedulesChanged();
}

void Endpoint::slotDisconnected() {
    cout<<__FUNCTION__<<" Alias "<<this->alias.toStdString()<<"\n";
    this->connected = false;
    if (clientSocket != NULL) {
        disconnect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
        disconnect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    }
}

void Endpoint::updateSocket(QTcpSocket* newSocket) {
    cout<<__FUNCTION__<<"Alias "<<this->alias.toStdString()<<"\n";
    this->clientSocket = newSocket;
    this->dataTransmitter->updateSocket(newSocket);
    this->connected = true;
    connect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));

    if(requestedState != state) {
        //a state change was requested when this endpoint was offline
        //now try to send request again
        requestState(requestedState);
    }
}

bool Endpoint::isConnected() {
    return this->connected;
}
void Endpoint::setConnected(bool connected){
    this->connected = connected;
}

QString Endpoint::getAlias() {
    return alias;
}
QString Endpoint::getType() {
    return type;
}
QString Endpoint::getMAC() {
    return MAC;
}
void Endpoint::setState(bool state) {
    this->state = state;

}
bool Endpoint::getState() {
    return this->state;
}

void Endpoint::setAuto(bool autoControlled)
{
    this->autoControlled = autoControlled;
    if (autoControlled == true &&
            autoControlledState != requestedState) {
        requestState(this->autoControlledState);
    }
}

bool Endpoint::isAutoControlled()
{
    return this->autoControlled;
}

bool Endpoint::ackIdentification()
{
    QByteArray payload;
    payload.append(getMAC());
    this->dataTransmitter->sendMessage(MESSAGETYPE_ENDPOINT_IDENT_ACK,payload);
    return true;
}

void Endpoint::requestState(bool state){
    //save the requested state for later
    requestedState = state;
    if (this->isConnected()) {
       //the local state is not changed
       //instead we wait for an update
       this->dataTransmitter->sendStateRequest(getMAC(), state);
    }else  {
        //Not connected at the moment
        //state change request will be send once endpoint reconnected (updateSocket())
    }
}
