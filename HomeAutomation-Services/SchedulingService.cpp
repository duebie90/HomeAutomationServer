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

void SchedulingService::setEndpoints(QList<Endpoint *> endpoints)
{
    //overwrite, just for the case there are new endpoints in the list
    this->endpoints = endpoints;
    foreach(Endpoint* endpoint, endpoints) {
        connect(endpoint, SIGNAL(signalSchedulesChanged()), this, SLOT(slotUpdateSchedules()));
    }
    slotUpdateSchedules();
}

void SchedulingService::slotUpdateSchedules()
{
    this->mapTimerToEndpoint.clear();
    this->mapTimerToEvent.clear();
    this->heartBeatTimer->stop();
    slotHeartBeat();
    this->heartBeatTimer->start();
}

void SchedulingService::slotHeartBeat()
{
    QTime now = QTime::currentTime();
    QDate todaysDate = QDate::currentDate();
    int secondsUntilStartEvent = -1;
    int secondsUntilEndEvent = -1;
    foreach(Endpoint* endpoint, this->endpoints)
    {
        bool takingPlaceToday = false;
        QList<ScheduleEvent*> events = endpoint->getScheduledEvents().values();
        foreach(ScheduleEvent* event, events) {
            switch(event->getRepetition()) {
            case ScheduleEvent::REPETITION_TYPE_NONE:
                //Fallthrough intended
            case ScheduleEvent::REPETITION_TYPE_DAYLY:
                takingPlaceToday = true;
                break;
            case ScheduleEvent::REPETITION_TYPE_DAYLY_ONLY_WEEKEND:
                //1=Monday
                if (todaysDate.dayOfWeek() == 6 || todaysDate.dayOfWeek() == 6) {
                    takingPlaceToday = true;
                }
                break;
            case ScheduleEvent::REPETITION_TYPE_DAYLY_WORKINGDAYS: {
                //1=Monday
                int dayNumber = todaysDate.dayOfWeek();
                if (todaysDate.dayOfWeek() != 6 && !todaysDate.dayOfWeek() != 6) {
                    takingPlaceToday = true;
                }
                break;}
            case ScheduleEvent::REPETITION_TYPE_WEEKLY:
                if (event->getWeekdays().at(todaysDate.dayOfWeek()-1) == true) {
                    takingPlaceToday = true;
                }
                break;
            default:
                qDebug()<<"Error on schedule Event: RepetitionType not recognized";
            }
            if (takingPlaceToday ) {
                int secondsUntilEvent = -1;
               // if(endpoint->getState() == false) {
                    //endpoint current in OFF state
                    secondsUntilStartEvent = now.secsTo(event->getStartTime());
               // } else {
                    //endpoint current in ON state
                    secondsUntilEndEvent = now.secsTo(event->getEndTime());
               // }
                bool takingPlaceWithingNextIntervall = false;
                if (secondsUntilStartEvent <= hearBeatIntervallSeconds && secondsUntilStartEvent >0 ) {
                    takingPlaceWithingNextIntervall = true;
                    event->setType(ScheduleEvent::EVENT_ON);
                    secondsUntilEvent = secondsUntilStartEvent;
                }else if (secondsUntilEndEvent <= hearBeatIntervallSeconds && secondsUntilEndEvent >0 ) {
                    takingPlaceWithingNextIntervall = true;
                    event->setType(ScheduleEvent::EVENT_OFF);
                    secondsUntilEvent = secondsUntilEndEvent;
                }
                if (takingPlaceWithingNextIntervall && secondsUntilEvent >0) {
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
    if( concerningEndpoint != NULL && event != NULL ) {
        concerningEndpoint->slotPerformEvent(event);
        this->mapTimerToEndpoint.remove(expiredTimer);
        this->mapTimerToEvent.remove(expiredTimer);
    }
}

