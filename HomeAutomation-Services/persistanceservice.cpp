#include "PersistanceService.h"
#include <QtSql>
#include <QDebug>
#include <QDir>
#include <homeautomationcontroller.h>

PersistanceService* PersistanceService::_instance = NULL;

PersistanceService::PersistanceService(QObject *parent) :
    QObject(parent),
    databaseReady(false),
    retryOpenDatabaseCounter(0)
{
    this->retryOpeningDatabaseTimer = new QTimer();
    connect(retryOpeningDatabaseTimer, SIGNAL(timeout()), this,
            SLOT(slotOpenDatabase()));
    slotOpenDatabase();
    qRegisterMetaType<QList<double> >("QList<double>");
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

QList<Endpoint *> PersistanceService::loadEndpoints()
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
    //
    //Request Schedules from DB; put them into the endpoint-objects too
    foreach(Endpoint* endpoint, endpoints) {
        //go through loadid endpoints and seek for schedule information
        QSqlQuery ScheduleQuery;
        ScheduleQuery.prepare("SELECT id, date, startTime, endTime, repetition, pendingEventType, mo, tu, we, th, fr, sa, so  FROM schedules WHERE macAdress == :mac");
        ScheduleQuery.bindValue(":mac", endpoint->getMAC());
        if(!ScheduleQuery.exec() ) {
            qDebug()<<"Error while getting endpoint information from database.";
            qDebug()<<"Last DB Error: "<<ScheduleQuery.lastError();
            return endpoints;
        }
        int idIndex = ScheduleQuery.record().indexOf("id");
        int dateIndex = ScheduleQuery.record().indexOf("date");
        int startTimeIndex = ScheduleQuery.record().indexOf("startTime");
        int endTimeIndex = ScheduleQuery.record().indexOf("endTime");
        int repetitionIndex = ScheduleQuery.record().indexOf("repetition");        
        int pendingTypeIndex = ScheduleQuery.record().indexOf("pendingEventType");
        int moIndex = ScheduleQuery.record().indexOf("mo");
        int tuIndex = ScheduleQuery.record().indexOf("tu");
        int soIndex = ScheduleQuery.record().indexOf("so");

        while (ScheduleQuery.next()) {
            int id= ScheduleQuery.value(idIndex).toInt();
            QTime startTime = QTime::fromString(ScheduleQuery.value(startTimeIndex).toString());
            QTime endTime = QTime::fromString(ScheduleQuery.value(endTimeIndex).toString());
            QDate eventDate = QDate::fromString(ScheduleQuery.value(dateIndex).toString());
            ScheduleEvent::RepetitionType repetition = ScheduleQuery.value(repetitionIndex).value<ScheduleEvent::RepetitionType>();
            ScheduleEvent::ScheduleEventType pendingEventType = ScheduleQuery.value(pendingTypeIndex).value<ScheduleEvent::ScheduleEventType>();
            //QVariant weekdaysVariant = ScheduleQuery.value(weekdaysIndex);
            QList<bool> weekdays;
            weekdays.append(ScheduleQuery.value(moIndex).toBool());
            weekdays.append(ScheduleQuery.value(tuIndex).toBool());
            weekdays.append(ScheduleQuery.value(moIndex+2).toBool());
            weekdays.append(ScheduleQuery.value(moIndex+3).toBool());
            weekdays.append(ScheduleQuery.value(moIndex+4).toBool());
            weekdays.append(ScheduleQuery.value(moIndex+5).toBool());
            weekdays.append(ScheduleQuery.value(moIndex+6).toBool());
            ScheduleEvent* newEvent = new ScheduleEvent(id, startTime, endTime, eventDate, repetition, pendingEventType, weekdays);
            endpoint->addScheduleEvent(newEvent);
        }
        this->endpoints.append(endpoint);
        this->mapMacToEndpoint.insert(endpoint->getMAC(), endpoint);
    }
    return endpoints;
}


PersistanceService *PersistanceService::getInstance()
{

    if (_instance == NULL) {
        _instance = new PersistanceService();
    }
    return _instance;
}

void PersistanceService::init()
{
    getInstance();
}

void PersistanceService::deInitiate()
{
    delete(_instance);
}

bool PersistanceService::addEndpoint(Endpoint *endpoint) {

    this->endpoints.append(endpoint);
    this->mapMacToEndpoint.insert(endpoint->getMAC(), endpoint);

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
    return this->endpoints;
}

Endpoint *PersistanceService::getEndpointByMac(QString mac)
{
    if (this->mapMacToEndpoint.contains(mac)) {
        //Endpoint exists --> get Pointer at it
        Endpoint* endpoint = this->mapMacToEndpoint.value(mac);
        return endpoint;
    }
    return NULL;
}

bool PersistanceService::deleteEndpoint(QString mac)
{
    Endpoint* endpoint = getEndpointByMac(mac);
    if  (endpoint != NULL) {
        if (!endpoint->isConnected()) {
            this->endpoints.removeOne(endpoint);
            this->mapMacToEndpoint.remove(mac);
        }
    }

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
    this->mapMacToEndpoint.clear();
    foreach(Endpoint* endpoint, this->endpoints) {
        delete endpoint;
    }
    endpoints.clear();

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

void PersistanceService::updateEndpointSchedule(QString mac, ScheduleEvent *event)
{
    //ToDo look into schedules table
    QSqlQuery query;
    if (!databaseReady) {
        cout<<"Error: database not ready. Settings will be lost after restart or power loss.\n";
        return;
    }
    query.prepare("SELECT id, macAdress FROM schedules WHERE id == :eventId AND macAdress == :mac");
    query.bindValue(":eventId", event->getId());
    query.bindValue(":mac", mac);
    if(!query.exec() ) {
        qDebug()<<"Error while getting endpoint information from database.";
        qDebug()<<"Last DB Error: "<<query.lastError();
        return;
    }
    //determine number of entries
    //query.size won't work on that DB
    int rowCount = 0;
    while(query.next()) {
        rowCount++;
    }

    if( rowCount == 0 ) {
        //the schedule doesnot seem to exist already-->add new
        //INSERT INTO schedules (spalten) VALUES (werte)
        query.prepare("INSERT INTO schedules (id, macAdress, date, startTime, endTime,"
                      "repetition, pendingEventType, mo, tu, we, th, fr, sa, so) VALUES "
                      "(:eventId, :macAdress, :date, :startTime, :endTime,"
                      ":repetition, :pendingEventType, :mo, :tu, :we, :th, :fr, :sa, :so)");
        query.bindValue(":eventId", event->getId());
        query.bindValue(":macAdress", mac);
        query.bindValue(":date", event->getDate().toString());
        query.bindValue(":startTime", event->getStartTime().toString());
        query.bindValue(":endTime", event->getEndTime().toString());
        query.bindValue(":repetition", (int)event->getRepetition());        
        query.bindValue(":pendingEventType", (int)event->getType());
        query.bindValue(":mo", QVariant::fromValue(event->getWeekdays().at(0)));
        query.bindValue(":tu", QVariant::fromValue(event->getWeekdays().at(1)));
        query.bindValue(":we", QVariant::fromValue(event->getWeekdays().at(2)));
        query.bindValue(":th", QVariant::fromValue(event->getWeekdays().at(3)));
        query.bindValue(":fr", QVariant::fromValue(event->getWeekdays().at(4)));
        query.bindValue(":sa", QVariant::fromValue(event->getWeekdays().at(5)));
        query.bindValue(":so", QVariant::fromValue(event->getWeekdays().at(6)));
        if(!query.exec() ) {
            qDebug()<<"Error while writing schedule getting endpoint information from database.";
            qDebug()<<"Last DB Error: "<<query.lastError();
            return;
        }

    } else if(rowCount == 1 ){
        //One entry exists-->update it
        query.prepare("UPDATE schedules SET date= :date, startTime = :startTime, endTime = :endTime, repetition = :repetition, pendingEventType = :pendingEventType, mo = :mo, tu = :tu, we = :we, th = :th, fr = :fr, sa = :sa, so = :so WHERE id == :eventId AND macAdress == :macAdress");
        query.bindValue(":eventId", event->getId());
        query.bindValue(":macAdress", mac);
        query.bindValue(":date", event->getDate().toString());
        query.bindValue(":startTime", event->getStartTime().toString());
        query.bindValue(":endTime", event->getEndTime().toString());
        query.bindValue(":repetition", (int)event->getRepetition());        
        query.bindValue(":pendingEventType", (int)event->getType());
        query.bindValue(":mo", QVariant::fromValue(event->getWeekdays().at(0)));
        query.bindValue(":tu", QVariant::fromValue(event->getWeekdays().at(1)));
        query.bindValue(":we", QVariant::fromValue(event->getWeekdays().at(2)));
        query.bindValue(":th", QVariant::fromValue(event->getWeekdays().at(3)));
        query.bindValue(":fr", QVariant::fromValue(event->getWeekdays().at(4)));
        query.bindValue(":sa", QVariant::fromValue(event->getWeekdays().at(5)));
        query.bindValue(":so", QVariant::fromValue(event->getWeekdays().at(6)));
        if(!query.exec() ) {
            qDebug()<<"Error while writing schedule getting endpoint information from database.";
            qDebug()<<"Last DB Error: "<<query.lastError();
            return;
        }
        qDebug()<<__FUNCTION__<<"update schedule id)"<<event->getId()<<" mac="<<mac<<" rows affected="<<query.numRowsAffected();
        //UPDATE TABLE schedules SET #spalte = #wert, #spalte = #wert WHERE ...
    } else  {
        qDebug()<<"Database error updating schedules. Value exists multiple times";
        //there are more than one hits:
        //Something is wrong with that DB -->repair it
        //DELETE ... FROM ... WHERE
    }


    //if schedules for this mac exist, compare schedule-id, eventually update
    //otherwise add entry



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
        createNewDb |=  !isSchedulesTablePresent();
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
        this->schedulesDb.close();
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
    QSqlQuery secondQuery;
    secondQuery.prepare("CREATE TABLE schedules(id integer primary key, macAdress text, date text, startTime text, endTime text, repetition int, pendingEventType integer, mo integer, tu integer, we integer, th integer, fr integer, sa integer, so integer)");
    if(!secondQuery.exec() ) {
        qDebug()<<"Error while sql query";
        qDebug()<<"Last DB Error: "<<secondQuery.lastError();
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

bool PersistanceService::isSchedulesTablePresent()
{
    QSqlQuery query;
    query.prepare("SELECT id, macAdress, date, startTime, endTime, repetition, pendingEventType, mo, tu, we, th, fr, sa, so FROM schedules");
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




