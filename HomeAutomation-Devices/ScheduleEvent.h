#ifndef SCHEDULEEVENT_H
#define SCHEDULEEVENT_H

#include <QObject>
#include <QTime>
#include <QDate>
#include <QDataStream>

class ScheduleEvent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString eventTypeString READ getTypeString NOTIFY signalEventChanged)
    Q_PROPERTY(QTime eventStartTime READ getStartTime NOTIFY signalEventChanged)
    Q_PROPERTY(QTime eventEndTime READ getEndTime NOTIFY signalEventChanged)
    Q_PROPERTY(int repetition READ getRepetition NOTIFY signalEventChanged)
    Q_PROPERTY(QList<bool> weekdays READ getWeekdays NOTIFY signalEventChanged)
public:
    //event type is obsolet:
    //ScheduleEvent now includes one switch on and one switch of time
    //ToDo remove
    enum ScheduleEventType {
        EVENT_ON,
        EVENT_OFF,
        EVENT_ANALOG,
        EVENT_NONE
    };
    Q_ENUMS(ScheduleEventType)


    enum RepetitionType {
        REPETITION_TYPE_NONE,
        REPETITION_TYPE_DAYLY,
        REPETITION_TYPE_DAYLY_WORKINGDAYS,
        REPETITION_TYPE_DAYLY_ONLY_WEEKEND,
        REPETITION_TYPE_WEEKLY
        //ToDo: invent more
    };
    Q_ENUMS(RepetitionType)
    explicit ScheduleEvent(int id=0, QTime startTime=QTime::currentTime(),QTime endTime=QTime::currentTime(), QDate date=QDate::currentDate(), ScheduleEvent::RepetitionType repetition=REPETITION_TYPE_NONE, ScheduleEvent::ScheduleEventType type=EVENT_OFF, QList<bool> weekdays=QList<bool>(), QObject *parent=0);
    ScheduleEvent(const ScheduleEvent& other);
    ~ScheduleEvent();
    int getId();
    void setId(int id);
    ScheduleEventType getType();
    void setType(ScheduleEventType);
    QString getTypeString();
    QTime getStartTime();
    QTime getEndTime();
    void setTime(QTime startTime, QTime endTime);
    //QString getTimeString();
    QDate getDate();
    void setDate(QDate date);
    RepetitionType getRepetition();
    bool hasRepetition();
    void setRepetition(RepetitionType repetition);
    //either recalculated day and time of processing or
    //clears pending flag (depending on repetition type)
    void setPerformed();
    bool isPending();

    QList<bool> getWeekdays();
    void setWeekdays(QList<bool> checkedWeekdays);
    QString toString();

    //friend QDataStream &ScheduleEvent::operator<<(QDataStream &ds, ScheduleEvent *obj)
    friend QDataStream &operator<<(QDataStream &ds, ScheduleEvent *obj)
    {
       ds<<(qint16)obj->getId();
       ds<<obj->getDate();
       ds<<obj->getStartTime();
       ds<<obj->getEndTime();
       ds<<(qint8)obj->getRepetition();
       ds<<(qint8)obj->getType();
       ds<<obj->weekdays;
       return ds;
    }
    //friend QDataStream &ScheduleEvent::operator>>(QDataStream &in,ScheduleEvent *sEvent)
    friend QDataStream &operator>>(QDataStream &in,ScheduleEvent *sEvent)
    {
       qint16 id;
       QDate date;
       QTime start, end;
       qint8 repetition;
       qint8 type;
       QList<bool> weekdays;
       //ScheduleEvent::RepetitionType repetition;
       //ScheduleEvent::ScheduleEventType type;
       in>>id>>date>>start>>end>>repetition>>type>>weekdays;
       sEvent->setId(id);
       sEvent->setDate(date);
       sEvent->setTime(start, end);
       sEvent->setRepetition((ScheduleEvent::RepetitionType)repetition);
       sEvent->setType((ScheduleEvent::ScheduleEventType)type);
       sEvent->setWeekdays(weekdays);
       return in;
    }


signals:
    void signalEventChanged();
public slots:

private:
    int id;
    //Time and date of next processing
    QDate date;

    QTime startTime;
    QTime endTime;

    //stores if another processing is required
    RepetitionType repetition;
    ScheduleEventType pendingEventType;
    //if repetition is weekly: active days can be chosen
    QList<bool> weekdays;

    bool pending;

};
 Q_DECLARE_METATYPE(ScheduleEvent)
 Q_DECLARE_METATYPE(ScheduleEvent::ScheduleEventType)
 Q_DECLARE_METATYPE(ScheduleEvent::RepetitionType)
#endif // SCHEDULEEVENT_H
