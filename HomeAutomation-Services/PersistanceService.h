#ifndef PERSISTANCESERVICE_H
#define PERSISTANCESERVICE_H

#include <QObject>
#include <QtSql>
#include <endpoint.h>
#include <QTimer>
class PersistanceService : public QObject
{
    Q_OBJECT
public:
    static PersistanceService* getInstance();
    void init();
    void deInitiate();
    bool addEndpoint(Endpoint* endpoint);
    QList<QString> getEndpointNames();
    //recover endpoint generell information as well as schedules from DB
    QList<Endpoint*> loadEndpoints();
    //returns the endpoints-pointer-list
    QList<Endpoint *> getEndpoints();

    Endpoint* getEndpointByMac(QString mac);
    bool deleteEndpoint(QString mac);
    void deleteEndpointsDatabase();

    void updateEndpointSchedule(QString mac, ScheduleEvent* event);
signals:

public slots:
private slots:
    void slotOpenDatabase();
private:
    explicit PersistanceService(QObject *parent = 0);
    ~PersistanceService();

    static PersistanceService* _instance;
    bool prepareSchedulesDb();
    bool isEndpointTablePresent();
    bool isSchedulesTablePresent();
    bool isTablePresent(QString tableName);
    int  getEndpointCount();
    QSqlDatabase schedulesDb;
    bool databaseReady;
    QTimer* retryOpeningDatabaseTimer;
    int retryOpenDatabaseCounter;

    //Instances of Endpoint or uiConnection are created
    //to handle identified client-connections
    QList<Endpoint*> endpoints;
    //QList<UiConnection*> uiConnections;
    //Map necessary for client recognition on reconnect

    QMap<QString, Endpoint*> mapMacToEndpoint;

};

#endif // PERSISTANCESERVICE_H
