#include "ScheduleEvent.h"


ScheduleEvent::ScheduleEvent(int id, QTime startTime, QTime endTime, QDate date, ScheduleEvent::RepetitionType repetition, ScheduleEvent::ScheduleEventType type, QList<bool> weekdays, QObject *parent):
    QObject(parent),
    startTime(startTime),
    endTime(endTime),
    date(date),
    repetition(repetition),
    pendingEventType(EVENT_NONE),
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
    return this->pendingEventType;
}

void ScheduleEvent::setType(ScheduleEvent::ScheduleEventType type)
{
    this->pendingEventType = type;
    emit signalEventChanged();
}

QString ScheduleEvent::getTypeString()
{
    if (this->pendingEventType == EVENT_ON) {
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
    switch(this->repetition) {
    case REPETITION_TYPE_NONE:
        eventString.append("Einmal ");
        break;
    case REPETITION_TYPE_DAYLY:
        eventString.append("Täglich ");
        break;
    case REPETITION_TYPE_DAYLY_ONLY_WEEKEND:
        eventString.append("Täglich am Wochenende ");
        break;
    case REPETITION_TYPE_DAYLY_WORKINGDAYS:
        eventString.append("Werktags ");
        break;
    case REPETITION_TYPE_WEEKLY:
        eventString.append("Wöchentlich ");
        break;
    default:
        ;
    }
    eventString.append(" EIN: " + this->startTime.toString("h:m") + "Uhr AUS: " +this->endTime.toString("h:m") + "Uhr" );

    return eventString;
}


