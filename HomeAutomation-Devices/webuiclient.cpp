#include "webuiclient.h"

class PersistanceService;

#include <../HomeAutomation-Services/PersistanceService.h>
#include <QDebug>
#include <endpoint.h>

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#define DEBUG_WEBUICLIENT false

WebUiClient::WebUiClient(QWebSocket* socket, QObject *parent) : QObject(parent)
{
    this->webSocket = socket;
    connect(socket, &QWebSocket::textMessageReceived, this, &WebUiClient::processTextMessage);
    connect(socket, &QWebSocket::binaryMessageReceived, this, &WebUiClient::processBinaryMessage);
    connect(socket, &QWebSocket::disconnected, this, &WebUiClient::socketDisconnected);
}

void WebUiClient::updateWebUi(QList<Endpoint *> endpoints)
{

    //create JSON-Object
    QJsonObject jRootObj;
    //create JSON Array
    QJsonArray jRootArr;
    //Put Values into JSON-Array


    foreach(Endpoint* endpoint, endpoints) {
        QString alias = endpoint->getAlias();
        //Insert Key Array Pair into Object
        QJsonObject jObj;
        jObj.insert("mac", QJsonValue(endpoint->getMAC()));
        jObj.insert("alias", QJsonValue(endpoint->getAlias()));
        jObj.insert("state", QJsonValue(endpoint->getState()));
        jRootArr << jObj;
    }

     //Insert Key Array Pair into Object
     jRootObj.insert("endpoints", QJsonValue(jRootArr));
    //create Document from Object
   // jRootObj.insert("endpoints", QJsonValue("abc"));


    QJsonDocument jDocument(jRootObj);
    //QString jsonString = jDocument.to
    QByteArray message = jDocument.toJson(QJsonDocument::Compact);
    QString messageString = QString(message);



    sendBinaryMessage(message);
}



void WebUiClient::processTextMessage(QString message)
{
    PersistanceService* ps = PersistanceService::getInstance();
    if(DEBUG_WEBUICLIENT) {
        qDebug()<<__FUNCTION__<<"from "<<this->webSocket->peerAddress()<<"; "<<this->webSocket->peerName();
    }
    //read message
    QList<QString>messageTokens = message.split(";");
    //get Json from text
    QJsonDocument jdok = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jobj = jdok.object();
    QString mac = jobj.value("mac").toString();
    bool requestedState = jobj.value("state").toBool();
    if (!mac.isEmpty()) {
        Endpoint* endpoint = ps->getEndpointByMac(mac);
        if (endpoint != NULL) {
            endpoint->requestState(requestedState);
        } else {
            if(DEBUG_WEBUICLIENT) {
                qDebug()<<__FUNCTION__<<"endpoint with mac "<<mac<<"not known";
            }
        }
    } else {
        if(DEBUG_WEBUICLIENT) {
            qDebug()<<__FUNCTION__<<"no mac recognized";
        }
    }
//    PersistanceService* ps = PersistanceService::getInstance();
//    QList<Endpoint*> endpointsList =ps->getEndpoints();
//    if (!endpointsList.isEmpty()) {
//        Endpoint* endpoint = endpointsList.first();
//        endpoint->requestState(message.contains("true"));
//    }
}

void WebUiClient::processBinaryMessage(QByteArray message)
{

}

void WebUiClient::socketDisconnected()
{
    emit disconnected();
}

bool WebUiClient::sendBinaryMessage(QByteArray message)
{
    if (this->webSocket != NULL && !message.isEmpty()) {
        //qint64 bytesSent = this->webSocket->sendBinaryMessage(message);
        qint64 bytesSent = this->webSocket->sendTextMessage(QString(message));
        return ((bytesSent > 0) ? true : false );
    }
    return false;
}
