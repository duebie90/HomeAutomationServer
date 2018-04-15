#include "AbstractEndpoint.h"

AbstractEndpoint::AbstractEndpoint(QString alias, QString type, QString MAC, QObject* parent)
    : QObject(parent)
{
    this->alias = alias;
    this->type = type;
    this->MAC = MAC;

}
AbstractEndpoint::AbstractEndpoint()
{

}

void AbstractEndpoint::copyEndpoint(AbstractEndpoint* otherEndpoint){

}

bool AbstractEndpoint:: isConnected(){
    return this->connected;
}

void AbstractEndpoint::setConnected(bool connected){
    this->connected = connected;
    emit signalUpdateEndpoint();
}

QString AbstractEndpoint::getAlias(){
    return this->alias;
}

QString AbstractEndpoint::getMAC(){
    return this->MAC;
}

QString AbstractEndpoint::getType(){
    return this->type;
}

void AbstractEndpoint::setState(bool state){
    this->state = state;
}

bool AbstractEndpoint::getState(){
    return this->state;
}

