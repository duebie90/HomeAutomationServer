#include "EndpointFactory.h"
#include <endpointtypes.h>

EndpointFactory* EndpointFactory::_instance = NULL;

EndpointFactory::EndpointFactory(QObject *parent) : QObject(parent)
{
    //Nothing to do yet
}

EndpointFactory* EndpointFactory::getInstance() {
    if (_instance == NULL) {
        _instance = new EndpointFactory();
    }
    return _instance;
}

AbstractEndpoint* EndpointFactory::getNewEndpointByType(EndpointTypes type){
    switch(type) {
    case ENDPOINT_TYPE_HEATING:
        return new HeatingEndpoint();
    break;
    case ENDPOINT_TYPE_SWITCHBOX:{
        return new Endpoint();
    }
    }
}

EndpointFactory::~EndpointFactory(){

}
