#include "ScheduleEvent.h"


ScheduleEvent::ScheduleEvent(QTime time, QDate date, ScheduleEvent::RepetitionType repetition, ScheduleEvent::ScheduleEventType type, QObject *parent):
    QObject(parent),
    time(time),
    date(date),
    repetition(repetition),
    eventType(type),
    pending(true)
{

}

ScheduleEvent::~ScheduleEvent()
{

}

ScheduleEvent::ScheduleEventType ScheduleEvent::getType()
{
    return this->eventType;
}

QTime ScheduleEvent::getTime()
{
    return time;
}

QDate ScheduleEvent::getDate()
{
    return date;
}

ScheduleEvent::RepetitionType ScheduleEvent::getRepetition()
{
    return repetition;
}


bool ScheduleEvent::hasRepetition()
{
    return (repetition != REPETITION_TYPE_NONE);
}

void ScheduleEvent::setPerformed()
{
    //ToDo: complete logic here
    switch(repetition) {
    case REPETITION_TYPE_DAYLY:
        this->date.addDays(1);
        break;
    case REPETITION_TYPE_DAYLY_WORKINGDAYS:
        //all day apart from saturday and sunday
        do {
            this->date.addDays(1);
        }while(this->date.dayOfWeek()==6 || this->date.dayOfWeek()==7);
        break;
    case REPETITION_TYPE_DAYLY_REAL_WORKINGSDAY:
        //all day apart from saturday and sunday
        do {
            this->date.addDays(1);
        }while(this->date.dayOfWeek()==7);
        break;
    case REPETITION_TYPE_WEEKLY:
        this->date.addDays(7);
        break;
    //ToDo: Weekends only, Sunday only, specific weekday, etc.
    case REPETITION_TYPE_NONE:
        break;
    default:
        break;
    }
    if (this->repetition != REPETITION_TYPE_NONE) {
        this->pending = true;
    } else {
        pending = false;
    }
}

bool ScheduleEvent::isPending()
{
    return pending;
}
