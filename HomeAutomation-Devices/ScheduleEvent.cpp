#include "ScheduleEvent.h"


ScheduleEvent::ScheduleEvent(int id, QTime startTime, QTime endTime, QDate date, ScheduleEvent::RepetitionType repetition, ScheduleEvent::ScheduleEventType type, QList<bool> weekdays, QObject *parent):
    QObject(parent),
    startTime(startTime),
    endTime(endTime),
    date(date),
    repetition(repetition),
    eventType(type),
    pending(true),
    id(id)
{
    if(weekdays.isEmpty()) {
        this->weekdays = {false, false, false, false, false, false, false};
    } else {
        this->weekdays = weekdays;
    }
}

ScheduleEvent::ScheduleEvent(const ScheduleEvent &other)
{

}

ScheduleEvent::~ScheduleEvent()
{

}

int ScheduleEvent::getId()
{
    return this->id;
}

void ScheduleEvent::setId(int id)
{
 this->id = id;
}

ScheduleEvent::ScheduleEventType ScheduleEvent::getType()
{
    return this->eventType;
}

void ScheduleEvent::setType(ScheduleEvent::ScheduleEventType type)
{
    this->eventType = type;
    emit signalEventChanged();
}

QString ScheduleEvent::getTypeString()
{
    if (this->eventType == EVENT_ON) {
        return "Einschalten";
    } else {
        return "Ausschalten";
    }
}

QTime ScheduleEvent::getStartTime()
{
    return this->startTime;
}

QTime ScheduleEvent::getEndTime()
{
    return this->endTime;
}

void ScheduleEvent::setTime(QTime startTime, QTime endTime)
{
    this->startTime = startTime;
    this->endTime   = endTime;
    emit signalEventChanged();
}

QDate ScheduleEvent::getDate()
{
    return date;
}

void ScheduleEvent::setDate(QDate date)
{
    this->date = date;
}

ScheduleEvent::RepetitionType ScheduleEvent::getRepetition()
{
    return repetition;
}


bool ScheduleEvent::hasRepetition()
{
    return (repetition != REPETITION_TYPE_NONE);
}

void ScheduleEvent::setRepetition(ScheduleEvent::RepetitionType repetition)
{
    this->repetition = repetition;
    emit signalEventChanged();
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
    case REPETITION_TYPE_DAYLY_ONLY_WEEKEND:
        //on saturday and sunday
        do {
            this->date.addDays(1);
        }while(!this->date.dayOfWeek()==6 || !this->date.dayOfWeek()==7);
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

QList<bool> ScheduleEvent::getWeekdays()
{
    return this->weekdays;
}

void ScheduleEvent::setWeekdays(QList<bool> checkedWeekdays)
{
    if(!checkedWeekdays.isEmpty()) {
        this->weekdays = checkedWeekdays;
    }
}
QString ScheduleEvent::toString()
{
    QString eventString;
    eventString.append(this->date.toString() + " EIN: " + this->startTime.toString() + " AUS: " +this->endTime.toString() );
    if(this->eventType == EVENT_ON) {
        eventString.append("EIN");
    } else {
        eventString.append("AUS");
    }
    return eventString;
}


