#include "endpoint.h"

Endpoint::Endpoint(QTcpSocket* socket, QString alias, QString type, QString MAC, QObject* parent):
    QObject(parent)
{
    cout<<"Endpoint-Object created with alias "<<alias.toStdString()<<" and type "<<type.toStdString()<<".\n";
    this->clientSocket = socket;
    this->alias = alias;
    this->type = type;
    this->MAC = MAC;
    this->state = false;
    this->dataReceiver = new DataReceiver();
    this->dataTransmitter = new DataTransmitter(socket);

    connect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));

    connect(dataReceiver, SIGNAL(signalReceivedEndpointState(QString,bool)), this, SLOT(slotReceivedState(QString,bool)));
    this->connected = true;
}

void Endpoint::slotReceivedState(QString MAC, bool state) {
    qDebug()<<__FUNCTION__<<"new State= "<<state;
    if(MAC == this->MAC) {
        setState(state);
    }
}

void Endpoint::slotStateRequested(bool state) {




}

void Endpoint::sendMessage(MessageType type, QByteArray message){
    this->dataTransmitter->sendMessage(type, message);
}

void Endpoint::slotDisconnected() {
    cout<<__FUNCTION__<<" Alias "<<this->alias.toStdString()<<"\n";
    this->connected = false;
    disconnect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    disconnect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

void Endpoint::updateSocket(QTcpSocket* newSocket) {
    cout<<__FUNCTION__<<"Alias "<<this->alias.toStdString()<<"\n";
    this->clientSocket = newSocket;
    this->dataTransmitter->updateSocket(newSocket);
    this->connected = true;
    connect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

bool Endpoint::isConnected() {
    return this->connected;
}
void Endpoint::setConnected(bool connected){
    this->connected = connected;
}

QString Endpoint::getAlias() {
    return alias;
}
QString Endpoint::getType() {
    return type;
}
QString Endpoint::getMAC() {
    return MAC;
}
void Endpoint::setState(bool state) {
    this->state = state;

}
bool Endpoint::getState() {
    return this->state;
}
void Endpoint::requestState(bool state){
    if (this->isConnected()) {
        if(this->alias != "DEBUG_ENDPOINT") {
            //if connected, send a request now
            QByteArray payload;
            payload.append(getMAC());
            payload.append(PDU_DELIMITER);
            payload.append(state ? "1": "0");
            sendMessage(MESSAGETYPE_SERVER_ENDPOINT_STATE_REQUEST,payload );
        }  else {
            //This is a simplified MSP430 Endpoint
            QByteArray message;
            if(state)
                message = "SWITCH_STATE=ON";
            else
                message = "SWITCH_STATE=OFF";
            message.append(0x0D);
            message.append(0x0A);
            this->clientSocket->write(message);
            this->state=state;
        }
    }else  {
        //Not connected at the moment
        //remember to do it later
        //ToDo: realy do it later
    }
}
