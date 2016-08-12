#include "UiDataReceiver.h"

#include <messagetype.h>
#include <iostream>
#include <QDebug>
#include <QTcpSocket>

using namespace std;

UiDataReceiver::UiDataReceiver(QObject* parent):
    QObject(parent)
{

}

void UiDataReceiver::slotReceivedData() {
    QTcpSocket* senderSocket = (QTcpSocket*)QObject::sender();
    QByteArray data = senderSocket->readAll();

    processProtocollHeader(senderSocket, data);
}

int UiDataReceiver::processProtocollHeader(QTcpSocket* socket, QByteArray data) {
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


void UiDataReceiver::processMessage(QTcpSocket* socket, MessageType type, QByteArray payload) {
    QString alias, MAC, endpointType, state, pass;
    bool switchedState;
    //0x1F = unit separator
    QList<QByteArray> payloadParts = payload.split(0x1F);

    switch(type) {
    case MESSAGETYPE_ENDPOINT_STATE:
        //Endpoint informs server about its state
        //or server requests endpoint to change its state
        if( payloadParts.length() < 2 ) {
            qDebug()<<"Faulty payload";
            return;
        }
        MAC     =   payloadParts.at(0);
        state = payloadParts.at(1);
        if(state == "1") {
            switchedState = true;
        }
        else {
                switchedState = false;
        }
        emit signalReceivedEndpointState(MAC, switchedState);
        break;
    case MESSAGETYPE_UI_ENDPOINT_STATE_REQUEST:
        if( payloadParts.length() < 2 ) {
            qDebug()<<"Faulty payload";
            return;
        }
        MAC     =   payloadParts.at(0);
        state = payloadParts.at(1);
        if(state == "1") {
            switchedState = true;
        }
        else {
                switchedState = false;
        }
        emit signalReceivedUiEndpointStateRequest(MAC, switchedState);
        break;
    case MESSAGETYPE_SERVER_ENDPOINT_STATE_REQUEST:
        if( payloadParts.length() < 2 ) {
            qDebug()<<"Faulty payload";
            return;
        }
        MAC     =   payloadParts.at(0);
        state = payloadParts.at(1);
        bool switchedState;
        if(state == "1") {
            switchedState = true;
        }
        else {
                switchedState = false;
        }
        emit signalReceivedServerEndpointStateRequest(MAC, switchedState);
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
    case MESSAGETYPE_UI_ENDPOINT_AUTO_REQUEST: {
        QString mac =payloadParts.at(0);
        bool autoControlled = false;
        if (payloadParts.at(1) == "1") {
            autoControlled = true;
            emit signalReceivedAutoRequest(mac, autoControlled);
        } else if(payloadParts.at(1) == "0"){
            autoControlled = false;
            emit signalReceivedAutoRequest(mac, autoControlled);
        }else {
            cout<<"Error: Faulty payload MESSAGETYPE_UI_ENDPOINT_AUTO\n";
        }
    }
        break;
    case MESSAGETYPE_RESET_SERVER:
        emit signalResetServer();
        qDebug()<<__FUNCTION__<<"Received server-reset message";
        break;
    case MESSAGETYPE_ENDPOINT_SCHEDULE:{
        QDataStream in(&payload, QIODevice::ReadOnly);
        in>>MAC;
        if (!MAC.isEmpty()) {
            ScheduleEvent* event = new ScheduleEvent();
            in>>event;
            emit signalReceivedEndpointSchedule(MAC, event);
        }
        break;
    }
    case MESSAGETYPE_UI_DELETE_ENDPOINT: {
        QString mac = payloadParts.at(0);
        emit signalDeleteEndpoint(mac);
    }
    break;
    case MESSAGETYPE_UI_DELETE_SCHEDULE: {
        QString mac = payloadParts.at(0);
        int id = payloadParts.at(1).toInt();
        qDebug()<<__FUNCTION__<<"UI Delete Schedule id: "<<id;
        emit signalDeleteSchedule(mac, id);

    }
    break;
    default:
        qDebug()<<__FUNCTION__<<"Unrecognized MessageType";
    }
}


