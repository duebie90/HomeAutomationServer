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
    dataReceiver = new DataReceiver();
    dataTransmitter = new DataTransmitter();

    connect(dataReceiver, SIGNAL(signalReceivedEndpointIdent(QTcpSocket*,QString,QString,QString)), this,
            SIGNAL(signalReceivedEndpointIdent(QTcpSocket*,QString,QString,QString)));
    connect(dataReceiver, SIGNAL(signalReceivedUiIdent(QTcpSocket*,QString, QString, QString)),
            this, SIGNAL(signalReceivedUiIdent(QTcpSocket*,QString, QString, QString)));

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
    disconnect(client, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    this->clientsPendingIdentification.removeOne(client);
}

void TcpServer::slotNetworkSessionOpened() {
    tcpServer = new QTcpServer(this);
    QObject::connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotClientConnected()));
    QObject::connect(tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(slotAcceptError()));

    if ( !tcpServer->listen(QHostAddress(serverAddress), serverPort)) {
        cout<<"Error: unable to start the tcp server";
    } else {
        cout<<"Server started: listening on port "<<tcpServer->serverPort()<<"\n";
        QString address = tcpServer->serverAddress().toString();
        cout<<"Server address "<<address.toStdString()<<"\n";
    }
}

void TcpServer::slotClientConnected() {
    QTcpSocket *clientSocket  = tcpServer->nextPendingConnection();

    //connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slotReceivedData()));
    QHostAddress remoteAddress = clientSocket->peerAddress();
    cout<<"A Client with IP "<<remoteAddress.toString().toStdString()<<" connected.\n";

    //we don't now how this is yet
    this->clientsPendingIdentification.append(clientSocket);
    //so we wait for him to send an identification message
    connect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    //but we let him now who we are
    QByteArray outData;
    //ToDo: send smth. appropriate
    outData.append("HaC:123456" );
    clientSocket->write(outData);

}

void TcpServer::slotAcceptError() {
    qDebug()<<"Error accepting client";
}




