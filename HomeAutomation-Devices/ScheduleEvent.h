#ifndef SCHEDULEEVENT_H
#define SCHEDULEEVENT_H

#include <QObject>
#include <QTime>
#include <QDate>

class ScheduleEvent : public QObject
{
    Q_OBJECT
public:
    enum ScheduleEventType {
        EVENT_ON,
        EVENT_OFF,
        EVENT_ANALOG
    };
    enum RepetitionType {
        REPETITION_TYPE_NONE,
        REPETITION_TYPE_DAYLY,
        REPETITION_TYPE_DAYLY_WORKINGDAYS,
        REPETITION_TYPE_DAYLY_REAL_WORKINGSDAY,
        REPETITION_TYPE_WEEKLY
        //ToDo: invent more
    };
    explicit ScheduleEvent(QTime time=QTime::currentTime(), QDate date=QDate::currentDate(), ScheduleEvent::RepetitionType repetition=REPETITION_TYPE_NONE, ScheduleEvent::ScheduleEventType type=EVENT_OFF, QObject *parent=0);
    ~ScheduleEvent();

    ScheduleEventType getType();
    QTime getTime();
    QDate getDate();
    RepetitionType getRepetition();
    bool hasRepetition();
    //either recalculated day and time of processing or
    //clears pending flag (depending on repetition type)
    void setPerformed();
    bool isPending();
signals:

public slots:

private:

    //Time and date of next processing
    QTime time;
    QDate date;
    //stores if another processing is required
    RepetitionType repetition;
    ScheduleEventType eventType;

    bool pending;

};

#endif // SCHEDULEEVENT_H
