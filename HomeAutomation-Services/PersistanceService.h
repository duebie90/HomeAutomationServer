#ifndef PERSISTANCESERVICE_H
#define PERSISTANCESERVICE_H

#include <QObject>
#include <QSqlDatabase>
#include "EndpointFactory.h"
#include <QTimer>
class PersistanceService : public QObject
{
    Q_OBJECT
public:
    static PersistanceService* getInstance();
    void init();
    void deInitiate();
    bool addEndpoint(AbstractEndpoint* endpoint);
    QList<QString> getEndpointNames();
    //recover endpoint generell information as well as schedules from DB
    QList<AbstractEndpoint*> loadEndpoints();
    //returns the endpoints-pointer-list
    QList<AbstractEndpoint *> getEndpoints();
    QList<Endpoint *> getSwitchboxEndpoints();
    int  getEndpointCount();
    AbstractEndpoint* getEndpointByMac(QString mac);
    bool deleteEndpoint(QString mac);
    void deleteEndpointsDatabase();

    void updateEndpointSchedule(QString mac, ScheduleEvent* event);

    void updateEndpoint(AbstractEndpoint* endpoint);
signals:

public slots:
private slots:
    void slotOpenDatabase();
private:
    explicit PersistanceService(QObject *parent = 0);
    ~PersistanceService();

    static PersistanceService* _instance;
    bool prepareTables();
    bool isEndpointTablePresent();
    bool isSchedulesTablePresent();
    bool isTablePresent(QString tableName);
    QSqlDatabase schedulesDb;
    bool databaseReady;
    QTimer* retryOpeningDatabaseTimer;
    int retryOpenDatabaseCounter;

    //Instances of Endpoint or uiConnection are created
    //to handle identified client-connections
    QList<AbstractEndpoint*> endpoints;
    //QList<UiConnection*> uiConnections;
    //Map necessary for client recognition on reconnect

    QMap<QString, AbstractEndpoint*> mapMacToEndpoint;

};

#endif // PERSISTANCESERVICE_H
