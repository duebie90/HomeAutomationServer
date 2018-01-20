#include "heatingendpoint.h"

HeatingEndpoint::HeatingEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC, QObject* parent):
Endpoint(socket, alias, type, MAC, parent)
{
    cout<<"HeatingEndpoint created."<<endl;
}

HeatingEndpoint::~HeatingEndpoint(){

}
