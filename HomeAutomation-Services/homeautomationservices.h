#ifndef HOMEAUTOMATIONSERVICES_H
#define HOMEAUTOMATIONSERVICES_H

#include <QObject>

class HomeAutomationServices : public QObject
{
    Q_OBJECT
public:
    explicit HomeAutomationServices(QObject *parent = 0);
    QString getString();
signals:

public slots:
};

#endif // HOMEAUTOMATIONSERVICES_H
