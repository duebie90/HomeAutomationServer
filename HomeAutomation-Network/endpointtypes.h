#ifndef ENDPOINTTYPES_H
#define ENDPOINTTYPES_H
#include <QObject>
enum EndpointTypes:unsigned int{
    ENDPOINT_TYPE_SWITCHBOX=0x21,
    ENDPOINT_TYPE_HEATING=0x22
};
Q_DECLARE_METATYPE(EndpointTypes)
#endif // ENDPOINTTYPES_H
