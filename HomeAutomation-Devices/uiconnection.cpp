#include "uiconnection.h"
#include <QtNetwork>


UiConnection::UiConnection(QTcpSocket* socket, QString alias, QObject* parent):
    QObject(parent)
{
    cout<<"UI Connection Object created with alias "<<alias.toStdString()<<".\n";
    this->clientSocket = socket;
    this->alias = alias;
    this->dataReceiver =  new DataReceiver();
    this->dataTransmitter = new DataTransmitter(socket);
    this->connected = true;
    connect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    //connections from data receiver
    connect(dataReceiver, SIGNAL(signalReceivedUiEndpointStateRequest(QString,bool)),
            this, SIGNAL(signalReceivedUiEndpointStateRequest(QString,bool)));
    connect(dataReceiver, SIGNAL(signalReceivedEndpointSchedule(QString,ScheduleEvent*)),
            this, SIGNAL(signalReceivedEndpointSchedule(QString,ScheduleEvent*)));


    connect(dataReceiver, SIGNAL(signalResetServer()), this, SIGNAL(signalResetServer()));
}
void UiConnection::sendMessage(QByteArray message){
    this->clientSocket->write(message, message.length());
}

void UiConnection::slotReceivedUiEndpointStateRequest(QString MAC, bool state) {


}

void UiConnection::slotDisconnected() {
    cout<<"Client Disconnected\n";
    this->connected = false;
}

