#include "homeautomationservices.h"

HomeAutomationServices::HomeAutomationServices(QObject *parent) : QObject(parent)
{

}

QString HomeAutomationServices::getString()
{
    return "Hallo Welt";
}

