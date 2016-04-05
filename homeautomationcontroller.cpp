#include "homeautomationcontroller.h"

#include <QtNetwork>
#include <iostream>

#define tempPassword "fhkiel"

using namespace std;

HomeAutomationController::HomeAutomationController(QObject *parent):
    QObject(parent)
{
    //this->hacUuid = new Uuid();
    this->pwd = tempPassword;
    tcpServer = new TcpServer("localhost", 3000);
    connect(tcpServer, SIGNAL(signalClientConnected(QTcpSocket*)), this, SLOT(slotClientConnected(QTcpSocket*)));
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
    connect(client, SIGNAL(readyRead()), SLOT(slotUnidentifiedClientMessage()));

    QByteArray outData;
    outData.append("HaC:123456" );
    client->write(outData);
}

void HomeAutomationController::slotUnidentifiedClientMessage() {
    QTcpSocket* senderSocket = (QTcpSocket*)QObject::sender();
    QByteArray data = senderSocket->readAll();
    QString message = QString(data);
    //Evaluate Massage according to ID-data
    if (message.contains("HaC-UI")) {
        processMessageNewUi(message, senderSocket);
    }else if(message.contains("HaC-Endpoint")) {
        processMessageNewEndpoint(message, senderSocket);
    } else {
        cout<<"HaC: Not identified data received: "<<message.toStdString()<<"\n";
    }
}

bool HomeAutomationController::processMessageNewEndpoint(QString message, QTcpSocket* socket) {
    QString alias="";
    QString type="";
    QString MAC="";
    //ToDo extract type=Enum
    cout<<"HaC: Endpoint recognized\n";
    //Preliminary Message structure
    //HaC-Endpoint_alias="abc"_type=switchbox_MAC=10:10:10:10:10:10

    QStringList messageParts = message.split("_", QString::SplitBehavior::SkipEmptyParts);
    if (messageParts.length()>=4) {
        QStringList splitOfAlias = messageParts.at(1).split("alias=", QString::SplitBehavior::SkipEmptyParts);
        if (splitOfAlias.length() > 0)
            alias = splitOfAlias.at(0);
        QStringList splitOfType = messageParts.at(2).split("type=", QString::SplitBehavior::SkipEmptyParts);
        if (splitOfType.length()>0)
            type = splitOfType.at(0);
        QStringList splitOfMAC = messageParts.at(3).split("MAC=", QString::SplitBehavior::SkipEmptyParts);
        if (splitOfMAC.length()>0) {
            if (splitOfMAC.at(0).length()==17) {
                MAC = splitOfMAC.at(0);
            }
        }
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
                } else {
                    cout<<"But alias information is different. It is therefore declined";
                }
            } else {
                addEndpoint(socket, alias, type, MAC);
            }
            return true;
        } else {
            cout<<"Massage invalid. Declined\n";
            return false;
        }
    }
    return false;
}

bool HomeAutomationController::processMessageNewUi(QString message, QTcpSocket* socket) {
    QString pass, alias, MAC;

    cout<<"HaC: Ui Controller recognized\n";
    //checking password also contained in this message

    //Preliminary Message structure
    //HaC-UI_alias=abc_pass=xyz_MAC=10:10:10:10:10:FF
    QStringList messageParts = message.split("_", QString::SplitBehavior::SkipEmptyParts);
    if (messageParts.length()>=4) {
        QStringList splitOfAlias = messageParts.at(1).split("alias=", QString::SplitBehavior::SkipEmptyParts);
        if (splitOfAlias.length() > 0)
            alias = splitOfAlias.at(0);
        QStringList splitOfPass = messageParts.at(2).split("type=", QString::SplitBehavior::SkipEmptyParts);
        if (splitOfPass.length()>0)
            pass = splitOfPass.at(0);
        QStringList splitOfMAC = messageParts.at(3).split("MAC=", QString::SplitBehavior::SkipEmptyParts);
        if (splitOfMAC.length()>0) {
            if (splitOfMAC.at(0).length()==17) {
                MAC = splitOfMAC.at(0);
            }
        }
        if (alias != "" && pass!="" && MAC != "") {

            bool passwordCorrect = (pass == this->pwd);
            if (passwordCorrect) {
                cout<<"Password valid. Access granted\n";
                addUiConnection(socket, alias);
                return true;
            }else {
                cout<<"Password invalid. Access denied\n";
                return false;
            }
        }else {
            cout<<"Massage invalid. Declined\n";
            return false;
        }
    }
}

void HomeAutomationController::addUiConnection(QTcpSocket* socket, QString alias) {

    UiConnection* newUiConnection = new UiConnection(socket, alias);
    uiConnections.append(newUiConnection);
    disconnect(socket, SIGNAL(readyRead()), this, SLOT(slotUnidentifiedClientMessage()));
    //dequeue unIdentified socket
    this->clientsPendingIdentification.removeOne(socket);
}

void HomeAutomationController::addEndpoint(QTcpSocket* socket, QString alias, QString type, QString MAC) {
    Endpoint* newEndpoint = new Endpoint(socket, alias, type);
    this->endpoints.append(newEndpoint);
    disconnect(socket, SIGNAL(readyRead()), this, SLOT(slotUnidentifiedClientMessage()));
    this->clientsPendingIdentification.removeOne(socket);
    this->mapMacToEndpoint.insert(MAC, newEndpoint);

}



