#include "EndpointDataReceiver.h"

#include <messagetype.h>
#include <iostream>
#include <QDebug>
#include <QTcpSocket>

using namespace std;

EndpointDataReceiver::EndpointDataReceiver(QObject* parent):
    QObject(parent)
{

}

void EndpointDataReceiver::slotReceivedData() {
    QTcpSocket* senderSocket = (QTcpSocket*)QObject::sender();
    QByteArray data = senderSocket->readAll();

    processProtocollHeader(senderSocket, data);
}

int EndpointDataReceiver::processProtocollHeader(QTcpSocket* socket, QByteArray data) {
    QByteArray payload, splitOfPayload;
    QList<QByteArray>  messageParts;
    MessageType messageType;
    quint16 payloadLength=0;


    //check if StartOfHeader Code is at(0)
    if (data.at(0) != 0x01) {
        cout<<"Received invalid message from "<<socket->peerAddress().toString().toStdString()<<"\n";
        cout<<"message-data: "<<data.toHex().toStdString()<<"\n";
        cout<<"Reason: missing 0x01 at index 0\n";
        return -1;
    }
    messageType = (MessageType)data.at(1); //second Byte
    QByteArray lengthBytes= data.mid(2,2);

    if(lengthBytes.at(0) != -1 && lengthBytes.at(0) != 0xFF) {
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
       cout<<"message-data: "<<data.toHex().toStdString()<<"\n";
        cout<<"Reason: 0x02 at end of header missing or no payload present\n";
        return -2;
    }
    if (messageType != MESSAGETYPE_ENDPOINT_SCHEDULE) {
        payload = splitOfPayload.split(0x03).at(0);
        //check payload length
        if (payload.length() != payloadLength) {
            cout<<"Received invalid message from "<<socket->peerAddress().toString().toStdString()<<"\n";
            cout<<"message-data: "<<data.toHex().toStdString()<<"\n";
            cout<<"Reason: message type "<<messageType<<": payload length from header "<<payloadLength<<" different from actual length: "<<payload.length()<<".\n";
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
            cout<<"message-data: "<<data.toHex().toStdString()<<"\n";
            cout<<"Reason termination malformed: "<<termination.toStdString()<<"\n";
            return -4;
        }
    cout<<"Received message from "<<socket->peerAddress().toString().toStdString()<<" Type: "<<messageType<<"\n";
    processMessage(socket, messageType, payload);
    return 0;
}


void EndpointDataReceiver::processMessage(QTcpSocket* socket, MessageType type, QByteArray payload) {
    QString alias, MAC, endpointType, state, pass;
    bool switchedState;
    //0x1F = unit separator
    QList<QByteArray> payloadParts = payload.split(0x1F);
    switch(type) {
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
    default:
        qDebug()<<__FUNCTION__<<"Unrecognized MessageType";
    }
}


