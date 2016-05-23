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
    clientSockets.append(clientSocket);

    emit signalClientConnected(clientSocket);
    //connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slotReceivedData()));
    QHostAddress remoteAddress = clientSocket->peerAddress();
    cout<<"A Client with IP "<<remoteAddress.toString().toStdString()<<" connected.\n";
}

void TcpServer::slotAcceptError() {
    qDebug()<<"Error accepting client";
}


