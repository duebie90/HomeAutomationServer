#ifndef ENDPOINTFACTORY_H
#define ENDPOINTFACTORY_H

#include <QObject>
#include <abstractendpoint.h>
#include <endpoint.h>
#include <heatingendpoint.h>
#include <endpointtypes.h>

class EndpointFactory : public QObject
{
    Q_OBJECT
public:    
    static EndpointFactory* getInstance();
    AbstractEndpoint* getNewEndpointByType(EndpointTypes type);

private:
    explicit EndpointFactory(QObject *parent = 0);
    ~EndpointFactory();
    static EndpointFactory* _instance;
};

#endif // ENDPOINTFACTORY_H
