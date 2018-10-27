#include "heatingendpoint.h"

HeatingEndpoint::HeatingEndpoint(QTcpSocket* socket, QString alias, EndpointTypes type, QString MAC, QObject* parent):
AbstractEndpoint(socket, alias, type, MAC, parent)
{
    cout<<"HeatingEndpoint created."<<endl;
}

HeatingEndpoint::~HeatingEndpoint(){

}

void HeatingEndpoint::serialize(QDataStream &ds){
    ds<<this->getAlias();
    ds<<this->getMAC();
    ds<<this->getState();
    ds<<this->isConnected();
}

void HeatingEndpoint::unserialize(QDataStream &ds){
    QString alias;
    QString mac;
    bool state;
    bool connected;
    ds>>alias>>mac>>state>>connected;

    this->setAlias(alias);
    this->setMAC(mac);
    this->setState(state);
    this->setConnected(connected);
}
