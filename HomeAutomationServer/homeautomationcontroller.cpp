#include "homeautomationcontroller.h"

#include <QtNetwork>
#include <iostream>
#include <UiDataReceiver.h>
#include <UiDataTransmitter.h>
#include <messagetype.h>
#include <PersistanceService.h>
#include <SchedulingService.h>


#define tempPassword "fhkiel"


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

    ps = new PersistanceService();
    ss = new SchedulingService();


    //Recover endpoint information from database
    QList<Endpoint*> recoveredEndpoints = ps->getEndpoints();
    if (recoveredEndpoints.length() > 0) {
        cout<<"Recovered "<<recoveredEndpoints.length()<<" endpoint-information from database\n";
        this->endpoints.append(recoveredEndpoints);
        foreach(Endpoint* endpoint, recoveredEndpoints) {
            this->mapMacToEndpoint.insert(endpoint->getMAC(), endpoint);
        }
    }
    ss->setEndpoints(this->endpoints);
}

HomeAutomationController::~HomeAutomationController() {
    endpointsPendingConfirmation.clear();
    endpoints.clear();
    uiConnections.clear();
    delete(tcpServer);
    delete(ps);
}

void HomeAutomationController::slotResetServer() {
    cout<<"Disconnecting all endpoints and resetting stored server data\n";
    //clear permanently stored data (ini or Sqlite)
    tcpServer->resetClientsPendingIdentification();
    endpointsPendingConfirmation.clear();
    this->mapMacToEndpoint.clear();
    foreach(Endpoint* endpoint, this->endpoints) {
        delete endpoint;
    }
    endpoints.clear();
    ps->deleteEndpointsDatabase();
    ss->setEndpoints(endpoints);
}

void HomeAutomationController::slotDeleteEndpoint(QString MAC) {
    cout<<"Deleting endpoint with MAC "<<MAC.toStdString()<<" was requested:";
    if (this->mapMacToEndpoint.contains(MAC)) {
        //Endpoint exists --> get Pointer at it
        Endpoint* endpoint = this->mapMacToEndpoint.value(MAC);
        this->endpoints.removeOne(endpoint);
        this->mapMacToEndpoint.remove(MAC);
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
        if(this->mapMacToEndpoint.contains(MAC)) {
            //The Endpoint is already known
            cout<<"Endpoint with alias "<<alias.toStdString()<<" has reconnected\n";

            Endpoint* reconnectedEndpoint;
            reconnectedEndpoint = this->mapMacToEndpoint.value(MAC);
            if (reconnectedEndpoint->getAlias() == alias) {
                reconnectedEndpoint->updateSocket(socket);
                reconnectedEndpoint->ackIdentification();
                //dequeue unIdentified socket
                tcpServer->clientIdentified(socket);
            } else {
                cout<<"But alias information is different. It is therefore declined\n";
            }
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
    cout<<"HaC: Ui Controller recognized\n";
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
    connect(newUiConnection, SIGNAL(signalReceivedUiEndpointStateRequest(QString,bool)),
            this, SLOT(slotForwardStateChangeRequest(QString,bool)));
    connect(newUiConnection, SIGNAL(signalResetServer()), this, SLOT(slotResetServer()));
    connect(newUiConnection, SIGNAL(signalReceivedEndpointSchedule(QString,ScheduleEvent*)),
            this, SLOT(slotForwardEndpointSchedule(QString,ScheduleEvent*)));
    connect(newUiConnection, SIGNAL(signalDisconnected()), this, SLOT(slotUiDisconnected()));
    connect(newUiConnection, SIGNAL(signalReceivedAutoRequest(QString,bool)),
            this, SLOT(slotForwardEndpointAutoRequest(QString,bool)));
}

void HomeAutomationController::addEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC) {
    Endpoint* newEndpoint = new Endpoint(socket, alias, type, MAC);
    this->endpoints.append(newEndpoint);
    this->mapMacToEndpoint.insert(MAC, newEndpoint);
    newEndpoint->ackIdentification();
    ss->setEndpoints(this->endpoints);
    ps->addEndpoint(newEndpoint);
}

void HomeAutomationController::slotUpdateUis() {
    foreach(UiConnection* uiConnection, this->uiConnections) {
        uiConnection->sendUpdate(this->endpoints);        
    }
}

void HomeAutomationController::slotForwardStateChangeRequest(QString MAC, bool state) {
    cout<<"Sending switch-";
    if(state)
        cout<<"on";
    else
        cout<<"off";
    cout<<"message to endpoint\n"<<MAC.toStdString()<<"\n";

    if (this->mapMacToEndpoint.contains(MAC)) {
        //Endpoint exists --> get Pointer at it
        Endpoint* endpoint = this->mapMacToEndpoint.value(MAC);
        endpoint->requestState(state);
    }else {
        qDebug()<<__FUNCTION__<<"Invalid MAC-Adress";
    }
}

void HomeAutomationController::slotForwardEndpointSchedule(QString MAC, ScheduleEvent* event)
{
    Endpoint* endpoint = this->mapMacToEndpoint.value(MAC);
    if(endpoint != NULL) {
        endpoint->updateScheduleEvent(event);
    }
}

void HomeAutomationController::slotForwardEndpointAutoRequest(QString MAC, bool autoMode)
{
    Endpoint* endpoint = this->mapMacToEndpoint.value(MAC);
    if(endpoint != NULL) {
        endpoint->setAuto(autoMode);
    }
}

