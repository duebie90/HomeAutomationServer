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
    explicit PersistanceService(QObject *parent = 0);
    ~PersistanceService();
    QList<QString> getEndpointNames();
    bool addEndpoint(Endpoint* endpoint);
signals:

public slots:
private slots:
    void slotOpenDatabase();
private:
    bool prepareSchedulesDb();
    bool isEndpointTablePresent();
    bool isTablePresent(QString tableName);
    int  getEndpointCount();
    QSqlDatabase schedulesDb;
    bool databaseReady;
    QTimer* retryOpeningDatabaseTimer;

};

#endif // PERSISTANCESERVICE_H
