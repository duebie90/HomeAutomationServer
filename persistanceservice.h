#ifndef PERSISTANCESERVICE_H
#define PERSISTANCESERVICE_H

#include <QObject>
#include <QtSql>
#include <endpoint.h>
class PersistanceService : public QObject
{
    Q_OBJECT
public:
    explicit PersistanceService(QObject *parent = 0);
    QList<QString> getEndpointNames();
    bool addEndpoint(Endpoint* endpoint);
signals:

public slots:

private:
    QSqlDatabase schedulesDb;

};

#endif // PERSISTANCESERVICE_H
