#ifndef HEATINGENDPOINT_H
#define HEATINGENDPOINT_H
#include <endpoint.h>
#include <QObject>

class HeatingEndpoint : public AbstractEndpoint
{
    Q_OBJECT
public:
    HeatingEndpoint(QTcpSocket* socket=nullptr, QString alias="", EndpointTypes type=ENDPOINT_TYPE_HEATING, QString MAC="", QObject* parent=nullptr);
    ~HeatingEndpoint();
    void serialize(QDataStream &ds);
    void unserialize(QDataStream &ds);
};

#endif // HEATINGENDPOINT_H
