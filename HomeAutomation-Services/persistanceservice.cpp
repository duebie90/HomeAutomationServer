#include "PersistanceService.h"
#include <QtSql>
#include <QDebug>
#include <QDir>
PersistanceService::PersistanceService(QObject *parent) :
    QObject(parent),
    databaseReady(false),
    retryOpenDatabaseCounter(0)
{
    this->retryOpeningDatabaseTimer = new QTimer();
    connect(retryOpeningDatabaseTimer, SIGNAL(timeout()), this,
            SLOT(slotOpenDatabase()));
    slotOpenDatabase();

}
PersistanceService::~PersistanceService()
{
    this->schedulesDb.close();
}

QList<QString> PersistanceService::getEndpointNames() {
    QList<QString> endpointNames;
    if (!databaseReady) {
        cout<<"Error: database not ready. Settings will be lost after restart or power loss.\n";
        return endpointNames;
    }
    QSqlQuery query;
    //getEndpointsQuery.prepare("SELECT name FROM endpoints");
    query.prepare("select * from endpoints");
    if(!query.exec() ) {
        qDebug()<<"Error while sql query";
        qDebug()<<"Last DB Error: "<<query.lastError();
        return endpointNames;
    }
    int nameId = query.record().indexOf("name");

    while (query.next()) {
        endpointNames.append(query.value(nameId).toString());
        qDebug()<<endpointNames.last();
    }
    return endpointNames;
}

bool PersistanceService::addEndpoint(Endpoint *endpoint) {
    if (!databaseReady) {
        cout<<"Error: database not ready. Settings will be lost after restart or power loss.\n";
        return false;
    }
    int id = getEndpointCount();
    QString name = endpoint->getAlias();
    QString MAC  = endpoint->getMAC();
    QString type = endpoint->getType();
    QSqlQuery query;
    query.prepare("INSERT INTO endpoints (id, name, macAdress, endpointType) VALUES (:id, :name, :mac, :type)");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":mac", MAC);
    query.bindValue(":type", type);
    if(!query.exec() ) {
        qDebug()<<"Error while sql query";
        qDebug()<<"Last DB Error: "<<query.lastError();
        return false;
    }
    return true;
}

QList<Endpoint *> PersistanceService::getEndpoints()
{
    QList<Endpoint*> endpoints;
    QSqlQuery query;
    if (!databaseReady) {
        cout<<"Error: database not ready. Settings will be lost after restart or power loss.\n";
        return endpoints;
    }
    query.prepare("SELECT id, name, macAdress, endpointType FROM endpoints");
    if(!query.exec() ) {
        qDebug()<<"Error while getting endpoint information from database.";
        qDebug()<<"Last DB Error: "<<query.lastError();
        return endpoints;
    }
    int nameIndex = query.record().indexOf("name");
    int macIndex = query.record().indexOf("macAdress");
    int typeIndex = query.record().indexOf("endpointType");
    while (query.next()) {
        QString alias = query.value(nameIndex).toString();
        QString type = query.value(typeIndex).toString();
        QString macAddress = query.value(macIndex).toString();
        Endpoint * newEndpoint = new Endpoint(NULL, alias, type, macAddress);
        newEndpoint->setConnected(false);
        endpoints.append(newEndpoint);
    }
    return endpoints;
}

bool PersistanceService::deleteEndpoint(QString mac)
{
    QSqlQuery query;
    if (!databaseReady) {
        cout<<"Error: database not ready. Settings will be lost after restart or power loss.\n";
        return false;
    }
    QString macWithQuotation =  "'" + mac + "'";
    query.prepare("DELETE FROM endpoints WHERE macAdress = (:macAdress)");
    query.bindValue(":macAdress", mac);
    if(!query.exec() ) {
        qDebug()<<"Error while getting endpoint information from database.";
        qDebug()<<"Last DB Error: "<<query.lastError();
        return false;
    }
    qDebug()<<"Last Query was"<<query.lastQuery();
    qDebug()<<"Last result was "<<query.lastError();

    return true;
}

void PersistanceService::deleteEndpointsDatabase()
{
    QFile dbFile;
    dbFile.setFileName(schedulesDb.databaseName());
    if (dbFile.exists()) {
        schedulesDb.close();
        if (dbFile.remove()) {
            cout<<"Database deleted";
            slotOpenDatabase();
        }
    }
}

void PersistanceService::slotOpenDatabase()
{
    bool openedDatabase = false;
    QFile dbFile;
    dbFile.setFileName(QCoreApplication::applicationDirPath() +"/schedulesDb.sqlite");
    bool createNewDb = !dbFile.exists();
    this->schedulesDb = QSqlDatabase::addDatabase("QSQLITE");    
    this->schedulesDb.setDatabaseName(QCoreApplication::applicationDirPath() +"/schedulesDb.sqlite");
    if (this->schedulesDb.open()) {
        qDebug()<<"PersistanceService: successfully opened schedulesDb.sqlite";
        openedDatabase = true;
    } else {
        qDebug()<<"PersistanceService: error opening DB schedulesDb.sqlite";
        cout<<"Warning database could not be opened.\n";
    }
    if (openedDatabase) {
        createNewDb |=  !isEndpointTablePresent();
        if (createNewDb) {
            if (prepareSchedulesDb()) {
                databaseReady = true;
                cout<<"No existing database found. Create a new one.";
            } else {
                databaseReady = false;
                cout<<"Warning database could not be created. Please check write access rights.";
            }
        } else {
            databaseReady = true;
        }
    } else {
        databaseReady = false;
    }
    if (!databaseReady && retryOpenDatabaseCounter != 100) {
        //schedule the next attempt
        this->retryOpeningDatabaseTimer->setInterval(2000);
        this->retryOpeningDatabaseTimer->setSingleShot(true);
        this->retryOpeningDatabaseTimer->start();
        this->retryOpenDatabaseCounter++;
    }
}

bool PersistanceService::prepareSchedulesDb()
{
    QSqlQuery query;
    //create empty "names" table for the first use
    //query.prepare("CREATE TABLE names(id integer primary key, name text, mac text, type text");
    query.prepare("CREATE TABLE endpoints(id integer primary key, name text, macAdress text, endpointType text)");
    if(!query.exec() ) {
        qDebug()<<"Error while sql query";
        qDebug()<<"Last DB Error: "<<query.lastError();
        return false;
    }
    return true;
}
bool PersistanceService::isEndpointTablePresent() {
    QSqlQuery query;
    query.prepare("SELECT id, name, macAdress, endpointType FROM endpoints");
    if(!query.exec() ) {
        qDebug()<<"Endpoints table does no exist or DB is not ready.\n";
        qDebug()<<"Last DB Error: "<<query.lastError()<<"\n";
        qDebug()<<"Details: "<<query.lastError().databaseText()<<" "<<query.lastError().type()<<" "<<query.lastError().text()<<"\n";
        return false;
    }
    return true;
}

bool PersistanceService::isTablePresent(QString tableName)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM :tableName");
    query.bindValue(":tableName", tableName);
    if(!query.exec() ) {
        qDebug()<<"Table "<<tableName<<"does no exist\n";
        return false;
    }
    return true;
}


int PersistanceService::getEndpointCount()
{
    int rowCount = 0;
    if (!databaseReady) {
        cout<<"Error: database not ready. Settings will be lost after restart or power loss.\n";
        return 0;
    }
    QSqlQuery query;
    query.prepare("select id from endpoints");
    if(!query.exec() ) {
        qDebug()<<"Error while sql query";
        qDebug()<<"Last DB Error: "<<query.lastError();
        return 0;
    }
    while(query.next()) {
        rowCount++;
    }

    return rowCount;

}




