#ifndef HEATINGENDPOINT_H
#define HEATINGENDPOINT_H
#include <endpoint.h>
#include <QObject>

class HeatingEndpoint : public Endpoint
{
    Q_OBJECT
public:
    HeatingEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC="", QObject* parent=0);
    ~HeatingEndpoint();
};

#endif // HEATINGENDPOINT_H
