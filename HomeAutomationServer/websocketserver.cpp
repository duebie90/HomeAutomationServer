#include "websocketserver.h"

#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <endpoint.h>
#include <PersistanceService.h>

WsServer::WsServer(QString address, quint16 port, QObject *parent) : QObject(parent)
{
    this->webSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                                 QWebSocketServer::NonSecureMode,
                                                 this);
     if (this->webSocketServer->listen(QHostAddress(address), port))
     {
         qDebug() << "WebsocketServer Server listening on port" << port;
         connect(this->webSocketServer, &QWebSocketServer::newConnection,
                 this, &WsServer::onNewConnection);
     }
}

WsServer::~WsServer()
{

}

void WsServer::slotUpdateWebUis()
{
    PersistanceService* ps = PersistanceService::getInstance();
    QList<Endpoint*> endpointsList =ps->getEndpoints();
    if (!endpointsList.isEmpty()) {
        foreach(WebUiClient* client, this->clients) {
            client->updateWebUi(endpointsList);
        }
    }
}

void WsServer::updateWebUi(WebUiClient* client)
{
    PersistanceService* ps = PersistanceService::getInstance();
    QList<Endpoint*> endpointsList =ps->getEndpoints();
    if (!endpointsList.isEmpty()) {
            client->updateWebUi(endpointsList);
    }
}

void WsServer::onNewConnection()
{
    QWebSocket *socket = this->webSocketServer->nextPendingConnection();
    WebUiClient* webUiClient = new WebUiClient(socket);
    connect(webUiClient, &WebUiClient::disconnected, this, &WsServer::clientDisconnected);
    this->clients << webUiClient;
    updateWebUi(webUiClient);
}
//void WsServer::processTextMessage(QString message)
//{
//    QWebSocket *senderSocket = qobject_cast<QWebSocket *>(sender());
//    PersistanceService* ps = PersistanceService::getInstance();
//    Endpoint* endpoint = ps->getEndpoints().first();
//    if (endpoint != NULL) {
//        endpoint->requestState(message.contains("1"));
//    }
//    //ToDo do smth.
//    //pClient->sendTextMessage(message);

//}
//void WsServer::processBinaryMessage(QByteArray message)
//{
//    QWebSocket *senderSocket = qobject_cast<QWebSocket *>(sender());
//    //ToDo do smth.
//    //pClient->sendTextMessage(message);
//}
void WsServer::clientDisconnected()
{
    WebUiClient *pClient = qobject_cast<WebUiClient *>(sender());
    if (pClient)
    {
        this->clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
