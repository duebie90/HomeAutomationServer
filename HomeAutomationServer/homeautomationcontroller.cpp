#include "homeautomationcontroller.h"

#include <QtNetwork>
#include <iostream>
#include <UiDataReceiver.h>
#include <UiDataTransmitter.h>
#include <messagetype.h>
#include <PersistanceService.h>
#include <SchedulingService.h>


#define tempPassword "fhkiel"

HomeAutomationController *HomeAutomationController::_instance = NULL;

using namespace std;

HomeAutomationController::HomeAutomationController(QObject *parent):
    QObject(parent)
{
    //this->hacUuid = new Uuid();
    this->pwd = tempPassword;
    //this->tcpServer = new TcpServer("127.0.0.1", 3000);
    this->tcpServer = new TcpServer("localhost", 3000);
    //this->dataReceiver = new DataReceiver();

    uiUpdateTimer = new QTimer();
    uiUpdateTimer->setInterval(500);
    uiUpdateTimer->start();
    connect(uiUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateUis()));

    connect(tcpServer, SIGNAL(signalReceivedEndpointIdent(QTcpSocket*,QString,QString,QString)), this,
            SLOT(slotProcessMessageNewEndpoint(QTcpSocket*,QString,QString,QString)));
    connect(tcpServer, SIGNAL(signalReceivedUiIdent(QTcpSocket*,QString, QString, QString)),
            this, SLOT( slotProcessMessageNewUi(QTcpSocket*,QString,QString, QString)));    

    ps = PersistanceService::getInstance();
    ss = new SchedulingService();


    //Recover endpoint information from database
    QList<Endpoint*> recoveredEndpoints = ps->loadEndpoints();
    if (recoveredEndpoints.length() > 0) {
        cout<<"Recovered "<<recoveredEndpoints.length()<<" endpoint-information from database\n";        
    }
    ss->setEndpoints(ps->getEndpoints());
}

HomeAutomationController::~HomeAutomationController() {
    endpointsPendingConfirmation.clear();
    //endpoints.clear();
    uiConnections.clear();
    delete(tcpServer);
    ps->deInitiate();
}

void HomeAutomationController::slotResetServer() {
    cout<<"Disconnecting all endpoints and resetting stored server data\n";
    //clear permanently stored data (ini or Sqlite)
    tcpServer->resetClientsPendingIdentification();
    endpointsPendingConfirmation.clear();
    ps->deleteEndpointsDatabase();
    ss->setEndpoints(ps->getEndpoints());
}

void HomeAutomationController::slotDeleteEndpoint(QString MAC) {
    cout<<"Deleting endpoint with MAC "<<MAC.toStdString()<<" was requested:";
    if (ps->getEndpointByMac(MAC) != NULL) {
        ps->deleteEndpoint(MAC);
        cout<<" done.\n";
    }else {
        cout<<"failed. Adress unknown.\n";
    }
}

void HomeAutomationController::slotUiDisconnected()
{
    UiConnection* disconnectedUi = (UiConnection*)QObject::sender();
    cout<<"Ui with IP "<<disconnectedUi->getIp().toString().toStdString()<<" disconnected\n";
    this->uiConnections.removeOne(disconnectedUi);
}

void HomeAutomationController::slotProcessMessageNewEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC) {
    cout<<"Endpoint identification received\n";
    if (alias != "" && type!="" && MAC != "") {
        //message seems to be valid
        //so check if that Endpoint is already known
        if(ps->getEndpointByMac(MAC) != NULL) {
            //The Endpoint is already known
            cout<<"Endpoint with alias "<<alias.toStdString()<<" has reconnected\n";

            Endpoint* reconnectedEndpoint;
            reconnectedEndpoint = ps->getEndpointByMac(MAC);
            reconnectedEndpoint->setAlias(alias);
            reconnectedEndpoint->updateSocket(socket);
            reconnectedEndpoint->ackIdentification();
            //dequeue unIdentified socket
            tcpServer->clientIdentified(socket);

        } else {
            addEndpoint(socket, alias, type, MAC);
            tcpServer->clientIdentified(socket);
        }
        //return true;
    } else {
        cout<<"Massage invalid. Declined\n";
        //return false;
    }

    //return false;
}

void HomeAutomationController::slotProcessMessageNewUi(QTcpSocket* socket, QString alias, QString pass, QString MAC) {
    cout<<"Ui Controller recognized\n";
    //checking password also contained in this message
    if (alias != "" && pass!="" && MAC != "") {
        bool passwordCorrect = (pass == this->pwd);
        if (passwordCorrect) {
            cout<<"Password valid. Access granted\n";
            addUiConnection(socket, alias);
            // return true;
        }else {
            cout<<"Password invalid. Access denied\n";
            // return false;
        }
    }else {
        cout<<"Massage invalid. Declined\n";
        //return false;
    }
}

void HomeAutomationController::addUiConnection(QTcpSocket* socket, QString alias) {

    UiConnection* newUiConnection = new UiConnection(socket, alias);
    uiConnections.append(newUiConnection);
    //dequeue unIdentified socket
    tcpServer->clientIdentified(socket);
    //connect signals
    connect(newUiConnection, SIGNAL(signalResetServer()), this, SLOT(slotResetServer()));
    connect(newUiConnection, SIGNAL(signalDisconnected()), this, SLOT(slotUiDisconnected()));
    connect(newUiConnection, SIGNAL(signalDeleteEndpoint(QString)), this, SLOT(slotForwardDeleteEndpoint(QString)));
    connect(newUiConnection, SIGNAL(signalDeleteSchedule(QString,int)), this, SLOT(slotForwardEndpointDeleteSchedule(QString,int)));
}

void HomeAutomationController::addEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC) {
    Endpoint* newEndpoint = new Endpoint(socket, alias, type, MAC);
    //this->endpoints.append(newEndpoint);
    //this->mapMacToEndpoint.insert(MAC, newEndpoint);
    newEndpoint->ackIdentification();
    ps->addEndpoint(newEndpoint);
    ss->setEndpoints(ps->getEndpoints());
}


HomeAutomationController *HomeAutomationController::getInstance()
{
    if (_instance == NULL) {
        _instance = new HomeAutomationController();
    }
    return _instance;
}

void HomeAutomationController::initiate()
{
    getInstance();
}

void HomeAutomationController::deInitialize()
{
    delete(_instance);
    //ToDo: stop services etc.
}

void HomeAutomationController::slotUpdateUis() {
    foreach(UiConnection* uiConnection, this->uiConnections) {
        uiConnection->sendUpdate(ps->getEndpoints());
    }
}

void HomeAutomationController::slotForwardDeleteEndpoint(QString mac)
{
    ps->deleteEndpoint(mac);
    //ToDo disconnect signals
}

void HomeAutomationController::slotForwardEndpointDeleteSchedule(QString mac, int id)
{
    Endpoint* endpoint = ps->getEndpointByMac(mac);
    if(endpoint != NULL) {
        endpoint->removeSchedule(id);
    }
}

