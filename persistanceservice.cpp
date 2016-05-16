#include "persistanceservice.h"
#include <QtSql>
#include <QDebug>
PersistanceService::PersistanceService(QObject *parent) :
    QObject(parent)
{
    this->schedulesDb = QSqlDatabase::addDatabase("QSQLITE");
    this->schedulesDb.setDatabaseName("./schedulesDb.sqlite");
    if(this->schedulesDb.open()) {
        qDebug()<<"PersistanceService: successfully opened schedulesDb.sqlite";
    } else {
        qDebug()<<"PersistanceService: error opening DB schedulesDb.sqlite";
    }

}

QList<QString> PersistanceService::getEndpointNames() {
QList<QString> endpointNames;
QSqlQuery getEndpointsQuery;
getEndpointsQuery.prepare("SELECT name FROM endpoints");
 if(!getEndpointsQuery.exec() ) {
     qDebug()<<"Error while sql query";
     return endpointNames;
  }
 int nameId = getEndpointsQuery.record().indexOf("name");
 while (getEndpointsQuery.next()) {
    endpointNames.append(getEndpointsQuery.value(nameId).toString());
    qDebug()<<endpointNames.last();
 }
 return endpointNames;
}

bool PersistanceService::addEndpoint(Endpoint *endpoint) {
    QString name = endpoint->getAlias();
    QString MAC  = endpoint->getMAC();
    QString type = endpoint->getType();
    QSqlQuery query;
    query.prepare("INSERT INTO endpoints (name, mac, type) VALUES (:name, :mac, :type)");
    query.bindValue(":name", name);
    query.bindValue(":mac", MAC);
    query.bindValue(":type", type);
    if(!query.exec() ) {
        qDebug()<<"Error while sql query";
        return false;
     }
    return true;
}


