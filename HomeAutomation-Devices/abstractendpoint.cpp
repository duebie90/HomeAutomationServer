#include "AbstractEndpoint.h"

// For debug purposes connection timeout can be deactivated
#define NO_TIMEOUT (true)

AbstractEndpoint::AbstractEndpoint(QTcpSocket* socket, QString alias, EndpointTypes type, QString MAC, QObject* parent)
    : QObject(parent)
{
    this->alias = alias;
    this->type = type;
    this->MAC = MAC;
    this->clientSocket = socket;
    this->dataReceiver = new EndpointDataReceiver();
    this->dataTransmitter = new EndpointDataTransmitter(socket);
    if(clientSocket != NULL && clientSocket != nullptr ) {
        connect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
        connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
        connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotSocketError(QAbstractSocket::SocketError)));
        //clientSocket->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant::fromValue(true));
    }

    connect(dataReceiver, SIGNAL(signalReceivedEndpointIdent(QTcpSocket*,QString,QString,QString)), this,
            SLOT(slotReceivedIdentMessage(QTcpSocket*,QString,QString,QString)));
    connect(dataReceiver, SIGNAL(signalReceivedHeartbeat()), this, SLOT(slotResetTimeout()));
    this->connected = true;

    this->keepAliveTimeoutTimer = new QTimer();
    this->keepAliveTimeoutTimer->setInterval(KEEP_ALIVE_TIMEOUT_MS);
    this->keepAliveTimeoutTimer->setSingleShot(true);
    connect(this->keepAliveTimeoutTimer, SIGNAL(timeout()), this, SLOT(slotKeepAliveTimeout()));
}

AbstractEndpoint::AbstractEndpoint()
{

}

void AbstractEndpoint::setState(bool state){
    this->state = state;
}

void AbstractEndpoint::updateSocket(QTcpSocket *newSocket){
    cout<<__FUNCTION__<<"Alias "<<this->alias.toStdString()<<"\n";
    this->clientSocket = newSocket;
    this->dataTransmitter->updateSocket(newSocket);
    connect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotSocketError(QAbstractSocket::SocketError)));
    clientSocket->setSocketOption(QAbstractSocket::KeepAliveOption, QVariant::fromValue(true));
    setConnected(true);
    this->keepAliveTimeoutTimer->start();
}

bool AbstractEndpoint::ackIdentification(){
    QByteArray payload;
    payload.append(getMAC());
    this->dataTransmitter->sendMessage(MESSAGETYPE_ENDPOINT_IDENT_ACK,payload);
    return true;
}

void AbstractEndpoint::sendMessage(MessageType type, QByteArray message){
    this->dataTransmitter->sendMessage(type, message);
}

bool AbstractEndpoint::getState(){
    return this->state;
}

QString AbstractEndpoint::getAlias(){
    return this->alias;
}

void AbstractEndpoint::setAlias(QString alias){
    this->alias = alias;
}

QString AbstractEndpoint::getMAC(){
    return this->MAC;
}

void AbstractEndpoint::setMAC(QString MAC){
    this->MAC = MAC;
}

bool AbstractEndpoint::isConnected(){
    return this->connected;
}

void AbstractEndpoint::setConnected(bool connected){
    this->connected = connected;
}

EndpointTypes AbstractEndpoint::getType(){
    return this->type;
}

void AbstractEndpoint::slotReceivedIdentMessage(QTcpSocket *socket, QString alias, QString type, QString MAC){
    Q_UNUSED(socket)
    Q_UNUSED(alias)
    Q_UNUSED(type)
    if(QString::compare(MAC, this->getMAC(),Qt::CaseSensitive) == 0) {
        ackIdentification();
    }
}

void AbstractEndpoint::slotDisconnected(){
    cout<<__FUNCTION__<<" Alias "<<this->alias.toStdString()<<"\n";
    setConnected(false);
    if (clientSocket != NULL) {
        disconnect(clientSocket, SIGNAL(readyRead()), dataReceiver, SLOT(slotReceivedData()));
        disconnect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    }
    emit signalConnectedChanged();
}

void AbstractEndpoint::slotSocketError(QAbstractSocket::SocketError socketError){
    switch (socketError) {
     case QAbstractSocket::ConnectionRefusedError:
          cout<<"Endpoint " + getAlias().toStdString() + "Socket Error: Connection refused \n";
          break;
     case QAbstractSocket::RemoteHostClosedError:
        this->clientSocket->close();
        break;
     default:
        cout<<"Endpoint " + getAlias().toStdString() + "Socket Error: \n";
        break;
    }
}

void AbstractEndpoint::slotResetTimeout()
{
    this->keepAliveTimeoutTimer->stop();
    this->keepAliveTimeoutTimer->start();
}

void AbstractEndpoint::slotKeepAliveTimeout()
{
    if(!NO_TIMEOUT) {
        cout<<"Endpoint "<<getAlias().toStdString()<<" disconnected. (Timeout)\n";
        setConnected(false);
        if(this->clientSocket != NULL ) {
            this->clientSocket->close();
        }
    }
}

