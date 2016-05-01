#include "homeautomationcontroller.h"

#include <QtNetwork>
#include <iostream>
#include <datareceiver.h>
#include <datatransmitter.h>
#include <messagetype.h>

#define tempPassword "fhkiel"


using namespace std;

HomeAutomationController::HomeAutomationController(QObject *parent):
    QObject(parent)
{
    //this->hacUuid = new Uuid();
    this->pwd = tempPassword;
    this->tcpServer = new TcpServer("localhost", 3000);
    this->dataReceiver = new DataReceiver();
    this->dataTransmitter = new DataTransmitter();
    connect(tcpServer, SIGNAL(signalClientConnected(QTcpSocket*)), this, SLOT(slotClientConnected(QTcpSocket*)));
    uiUpdateTimer = new QTimer();
    uiUpdateTimer->setInterval(500);
    uiUpdateTimer->start();
    connect(uiUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateUis()));

    connect(dataReceiver, SIGNAL(signalReceivedEndpointIdent(QTcpSocket*,QString,QString,QString)), this,
            SLOT(slotProcessMessageNewEndpoint(QTcpSocket*,QString,QString,QString)));
    connect(dataReceiver, SIGNAL(signalReceivedUiIdent(QTcpSocket*,QString, QString, QString)),
            this, SLOT( slotProcessMessageNewUi(QTcpSocket*,QString,QString, QString)));
    //create settings object for persistant data storage
    this->settings.beginGroup("MainControler");
    this->settings.setValue("test", 21);
    this->settings.endGroup();
}

HomeAutomationController::~HomeAutomationController() {
    clientsPendingIdentification.clear();
    endpointsPendingConfirmation.clear();
    endpoints.clear();
    uiConnections.clear();
    delete(tcpServer);
}


void HomeAutomationController::slotClientConnected(QTcpSocket* client) {
    this->clientsPendingIdentification.append(client);
    connect(client, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    QByteArray outData;
    outData.append("HaC:123456" );
    client->write(outData);
    this->settings.beginGroup("MainControler");
    int test = this->settings.value("test").toInt();
    this->settings.endGroup();
    cout<<"SettingsValue test="<<QString::number(test).toStdString()<<"\n";
}

void HomeAutomationController::slotProcessMessageNewEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC) {
    cout<<"HaC: Endpoint recognized\n";

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
                disconnect(socket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
                //dequeue unIdentified socket
                this->clientsPendingIdentification.removeOne(socket);
            } else {
                cout<<"But alias information is different. It is therefore declined";
            }
        } else {
            addEndpoint(socket, alias, type, MAC);
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
    disconnect(socket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    //dequeue unIdentified socket
    this->clientsPendingIdentification.removeOne(socket);
    //connect signals
    connect(newUiConnection, SIGNAL(signalReceivedUiEndpointStateRequest(QString,bool)),
            this, SLOT(slotForwardStateChangeRequest(QString,bool)));
}

void HomeAutomationController::addEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC) {
    Endpoint* newEndpoint = new Endpoint(socket, alias, type, MAC);
    this->endpoints.append(newEndpoint);
    disconnect(socket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    this->clientsPendingIdentification.removeOne(socket);
    this->mapMacToEndpoint.insert(MAC, newEndpoint);
}

void HomeAutomationController::slotUpdateUis() {
    QByteArray message, payload="";
    if(this->endpoints.length() > 0) {
        foreach(Endpoint* endpoint, this->endpoints ) {
            if(payload !="") {
                payload.append(PDU_DELIMITER);
            }
            payload.append(endpoint->getAlias());
            payload.append(PDU_DELIMITER);
            payload.append(endpoint->getMAC());
            payload.append(PDU_DELIMITER);
            payload.append(endpoint->getType());
            payload.append(PDU_DELIMITER);
            bool state = endpoint->getState();
            payload.append(state ? "1": "0");
            payload.append(PDU_DELIMITER);
            bool connected = endpoint->isConnected();
            payload.append(connected ? "1": "0");
        }
        if(payload != "") {
            message = dataTransmitter->prepareMessage(MESSAGETYPE_ENDPOINTS_STATES_LIST, payload);
            foreach(UiConnection* uiConnection, this->uiConnections) {
                uiConnection->sendMessage(message);
            }
        }
    }
}

void HomeAutomationController::slotForwardStateChangeRequest(QString MAC, bool state) {
    cout<<"Sending switch-";
    if(state)
        cout<<"on";
    else
        cout<<"off";
    cout<<"message to endpoint\n"<<MAC.toStdString();

    if (this->mapMacToEndpoint.contains(MAC)) {
        //Endpoint exists --> get Pointer at it
        Endpoint* endpoint = this->mapMacToEndpoint.value(MAC);
        endpoint->requestState(state);
    }else {
        qDebug()<<__FUNCTION__<<"Invalid MAC-Adress";
    }
}


