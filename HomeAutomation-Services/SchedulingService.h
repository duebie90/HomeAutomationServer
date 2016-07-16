#ifndef SCHEDULINGSERVICE_H
#define SCHEDULINGSERVICE_H

#include <QObject>
#include <endpoint.h>
#include <QTimer>
#include <ScheduleEvent.h>

class SchedulingService : public QObject
{
    Q_OBJECT
public:
    explicit SchedulingService(QObject *parent = 0);
    void setEndpoints(QList<Endpoint*> endpoints);
signals:
    void signalSchedulesChanged();
public slots:
private slots:
    void slotUpdateSchedules();
    void slotHeartBeat();
    void slotPerformEvent();
private:
    QTimer* heartBeatTimer;
    QList<Endpoint*> endpoints;
    QMap<QTimer*, Endpoint* > mapTimerToEndpoint;
    QMap<QTimer*, ScheduleEvent*> mapTimerToEvent;
    int hearBeatIntervallSeconds;
};

#endif // SCHEDULINGSERVICE_H
