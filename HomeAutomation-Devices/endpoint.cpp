#include "endpoint.h"
#include <string>
class PersistanceService;

#include <../HomeAutomation-Services/PersistanceService.h>

#define NO_TIMEOUT (true)

Endpoint::Endpoint(QTcpSocket* socket, QString alias, EndpointTypes type, QString MAC, QObject* parent):
    AbstractEndpoint(socket, alias, type, MAC, parent)
{
    cout<<"Endpoint-Object created with alias "<<alias.toStdString()<<" and type "<<QString::number(type).toStdString()<<".\n";
    this->state = false;    
    this->requestedState = false;
    this->autoControlled = false;
    this->autoControlledState = false;
    this->stateChangePending = false;    
    connect(dataReceiver, SIGNAL(signalReceivedEndpointState(QString,bool)), this, SLOT(slotReceivedState(QString,bool)));
    connect(dataReceiver, SIGNAL(signalReceivedEndpointIdent(QTcpSocket*,QString,QString,QString)), this,
            SLOT(slotReceivedIdentMessage(QTcpSocket*,QString,QString,QString)));        
}

void Endpoint::serialize(QDataStream &ds){
    ds<<this->getAlias();
    ds<<this->getMAC();
    ds<<this->getState();
    ds<<this->isAutoControlled();
    ds<<this->isConnected();
    ds<<this->isStateChangePending();
}

void Endpoint::unserialize(QDataStream &ds){
    ds<<this->getAlias();
    ds<<this->getMAC();
    ds<<this->getState();
    ds<<this->isAutoControlled();
    ds<<this->isConnected();
    ds<<this->isStateChangePending();
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

    //qDebug()<<__FUNCTION__<<"new State= "<<state;

    if(MAC == this->MAC) {
        this->stateChangePending = false;
        slotResetTimeout();
        setState(state);
        AbstractEndpoint* ae = dynamic_cast<AbstractEndpoint*>(this);
        PersistanceService::getInstance()->updateEndpoint(ae);
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

QMap<int, ScheduleEvent*> Endpoint::getScheduledEvents()
{
    return this->scheduleEvents;
}

void Endpoint::addScheduleEvent(ScheduleEvent *event)
{
    this->scheduleEvents.insert(event->getId(), event);
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
        return;
    }
    PersistanceService::getInstance()->updateEndpointSchedule(getMAC(),event);
    emit signalSchedulesChanged();
}

void Endpoint::removeSchedule(int id)
{
    ScheduleEvent* event = this->scheduleEvents.value(id);
    if(event != NULL) {
        this->scheduleEvents.remove(id);
    }
    emit signalSchedulesChanged();
}

bool Endpoint::isStateChangePending()
{
    return this->stateChangePending;
}

void Endpoint::updateSocket(QTcpSocket* newSocket) {    
    AbstractEndpoint::updateSocket(newSocket);
    if(requestedState != state) {
        //a state change was requested when this endpoint was offline
        //now try to send request again
        requestState(requestedState);
    }    
}

bool Endpoint::getRequestedState()
{
    return this->requestedState;
}

void Endpoint::setAuto(bool autoControlled)
{
    this->autoControlled = autoControlled;
    emit signalStateChanged();
    AbstractEndpoint* ae = dynamic_cast<AbstractEndpoint*>(this);
    PersistanceService::getInstance()->updateEndpoint(ae);
    if (autoControlled == true &&
            autoControlledState != requestedState) {
        requestState(this->autoControlledState);
    }
}

bool Endpoint::isAutoControlled()
{
    return this->autoControlled;
}


void Endpoint::requestState(bool state){
    //save the requested state for later
    requestedState = state;
    if (this->isConnected()) {
       //the local state is not changed
       //instead we wait for an update
       this->dataTransmitter->sendStateRequest(getMAC(), state);
       this->stateChangePending =true;
    }else  {
        //Not connected at the moment
        //state change request will be send once endpoint reconnected (updateSocket())
    }
    AbstractEndpoint* ae = dynamic_cast<AbstractEndpoint*>(this);
    PersistanceService::getInstance()->updateEndpoint(ae);
}
