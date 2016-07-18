#include "tcpserver.h"

#include <QtNetwork>

//#include <QTcpServer>
//#include <QNetworkSession>
//#include <QNetworkConfigurationManager>
//#include <QTcpSocket>

#include <iostream>
using namespace std;

TcpServer::TcpServer(QString address, int port, QObject *parent):
    QObject(parent),
    tcpServer(0),
    session(0),
    serverAddress(address),
    serverPort(port)
{
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired ) {
        //Network session seems to be necessary, so make one
        QNetworkConfiguration config = manager.defaultConfiguration();        
        session = new QNetworkSession(config);
        //notify me once the session is open
        QObject::connect(session, SIGNAL(opened()), this, SLOT(slotNetworkSessionOpened()));
    }else {
        //otherwise start server directly
        slotNetworkSessionOpened();
    }

}


TcpServer::~TcpServer() {
    clientSockets.clear();
    clientsPendingIdentification.clear();
}

void TcpServer::resetClientsPendingIdentification()
{
    this->clientsPendingIdentification.clear();
}

void TcpServer::clientIdentified(QTcpSocket *client)
{
    disconnect(client, SIGNAL(readyRead()), this, SLOT(slotReceivedData()));
    this->clientsPendingIdentification.removeOne(client);
}

void TcpServer::slotNetworkSessionOpened() {
    tcpServer = new QTcpServer(this);
    QObject::connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotClientConnected()));
    QObject::connect(tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(slotAcceptError()));

    if ( !tcpServer->listen(QHostAddress(serverAddress), serverPort)) {
        cout<<"Error: unable to start the tcp server\n";
    } else {
        cout<<"Server started: listening on port "<<tcpServer->serverPort()<<"\n";
        QString address = tcpServer->serverAddress().toString();
        cout<<"Server address "<<address.toStdString()<<"\n";
    }
}

void TcpServer::slotClientConnected() {
    QTcpSocket *clientSocket  = tcpServer->nextPendingConnection();
    //clientSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    QHostAddress remoteAddress = clientSocket->peerAddress();
    cout<<"A Client with IP "<<remoteAddress.toString().toStdString()<<" connected.\n";

    //we don't now who this is yet
    this->clientsPendingIdentification.append(clientSocket);
    //so we wait for him to send an identification message
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slotReceivedData()));
    //but we let him now who we are
    //ToDo send server info

}

void TcpServer::slotAcceptError() {
    qDebug()<<"Error accepting client";
}

void TcpServer::slotReceivedData()
{
    QTcpSocket* senderSocket = (QTcpSocket*)QObject::sender();
    QByteArray data = senderSocket->readAll();
    processProtocollHeader(senderSocket, data);
}

int TcpServer::processProtocollHeader(QTcpSocket *socket, QByteArray data)
{
    QByteArray payload, splitOfPayload;
    QList<QByteArray>  messageParts;
    MessageType messageType;
    quint16 payloadLength=0;

    //check if StartOfHeader Code is at(0)
    if (data.at(0) != 0x01) {
        cout<<"Received invalid message from "<<socket->peerAddress().toString().toStdString()<<"\n";
        return -1;
    }
    messageType = (MessageType)data.at(1); //second Byte
    QByteArray lengthBytes= data.mid(2,2);

    if(lengthBytes.at(0) != -1) {
        //0xFF stands for 0x00, 0 makes trouble in c strings
        payloadLength |= (quint8)(lengthBytes.at(0));
    }

    payloadLength = payloadLength << 8;
    payloadLength |= (quint8)(lengthBytes.at(1));

    //get payload section (from 0x02 ..0x03)
    messageParts = data.split(0x02);
    if ( messageParts.length() >= 2 ) {
        splitOfPayload = messageParts.at(1);
    } else {
        cout<<"Received invalid message from "<<socket->peerAddress().toString().toStdString()<<"\n";
        return -2;
    }
    if (messageType != MESSAGETYPE_ENDPOINT_SCHEDULE) {
        payload = splitOfPayload.split(0x03).at(0);
        //check payload length
        if (payload.length() != payloadLength) {
            cout<<"Received invalid message from "<<socket->peerAddress().toString().toStdString()<<"\n";
            return -3;
        }
    } else {
        payload = data.mid(5, payloadLength);
    }
    //check correct termination after payload section (0x03|0x04)
    QByteArray termination = data.mid(5 + payloadLength, 2);
    if (termination.length() >= 2 )
        if(termination.at(0) != 0x03 || termination.at(1) != 0x04 ) {
            cout<<"Received invalid message from "<<socket->peerAddress().toString().toStdString();
            return -4;
        }
    cout<<"Received message from "<<socket->peerAddress().toString().toStdString()<<" Type: "<<messageType<<"\n";
    processMessage(socket, messageType, payload);
    return 0;
}

void TcpServer::processMessage(QTcpSocket *socket, MessageType type, QByteArray payload)
{
    QList<QByteArray> payloadParts = payload.split(0x1F);
    QString alias, MAC, endpointType, pass;
    switch(type) {
    case MESSAGETYPE_UI_INFO:
        if( payloadParts.length() < 2 ) {
            qDebug()<<"Faulty payload";
            return;
        }
        alias   =   payloadParts.at(0);
        pass    =   payloadParts.at(1);
        MAC     =   payloadParts.at(2);
        emit signalReceivedUiIdent(socket, alias, pass, MAC);
    break;
    case MESSAGETYPE_ENDPOINT_IDENT:
        if( payloadParts.length() < 3 ) {
            qDebug()<<"Faulty payload";
            return;
        }
        alias   =   payloadParts.at(0);
        MAC     =   payloadParts.at(1);
        endpointType = payloadParts.at(2);

        emit signalReceivedEndpointIdent(socket, alias, endpointType, MAC);
        break;
    }
}






