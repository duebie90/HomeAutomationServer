#include "SchedulingService.h"
#include <QTimer>
#include <QTime>
#include <QDebug>
//Intervall of Heartbeats in minutes
#define HEARTBEAT_INTERVALL 1

SchedulingService::SchedulingService(QObject *parent) :
    QObject(parent),
    hearBeatIntervallSeconds(HEARTBEAT_INTERVALL*60)
{
    //At every "heartbeat", QTimers for the scheduling events within the next intervall are created
    heartBeatTimer = new QTimer();
    heartBeatTimer->setInterval(hearBeatIntervallSeconds*1000);
    heartBeatTimer->start();
    connect(heartBeatTimer, SIGNAL(timeout()), this, SLOT(slotHeartBeat()));

}

void SchedulingService::updateSchedule(QList<Endpoint *> endpoints)
{
    this->endpoints = endpoints;
    this->mapTimerToEndpoint.clear();
    this->mapTimerToEvent.clear();
    this->heartBeatTimer->stop();
    slotHeartBeat();
    this->heartBeatTimer->start();

}

void SchedulingService::slotHeartBeat()
{
    QTime now = QTime::currentTime();
    foreach(Endpoint* endpoint, this->endpoints)
    {
        QList<ScheduleEvent*> events = endpoint->getScheduledEvents();
        foreach(ScheduleEvent* event, events) {
            if(event->getDate() == QDate::currentDate()) {
                //it's today
                int secondsUntilEvent = now.secsTo(event->getTime());
                if(secondsUntilEvent <= hearBeatIntervallSeconds && secondsUntilEvent >0 ) {
                    QTimer* newTimer = new QTimer();
                    newTimer->setInterval(secondsUntilEvent*1000);
                    newTimer->setSingleShot(true);
                    this->mapTimerToEndpoint.insert(newTimer, endpoint);
                    connect(newTimer, SIGNAL(timeout()), this, SLOT(slotPerformEvent()));
                    this->mapTimerToEvent.insert(newTimer, event);
                    newTimer->start();
                }

            }

        }
        //create QTimers for all scheduled events of the next intervall
    }
}

void SchedulingService::slotPerformEvent()
{
    QTimer* expiredTimer = (QTimer*)QObject::sender();
    Endpoint* concerningEndpoint = this->mapTimerToEndpoint.value(expiredTimer);
    ScheduleEvent* event    = this->mapTimerToEvent.value(expiredTimer);
    concerningEndpoint->slotPerformEvent(event);
    this->mapTimerToEndpoint.remove(expiredTimer);
    this->mapTimerToEvent.remove(expiredTimer);

}

