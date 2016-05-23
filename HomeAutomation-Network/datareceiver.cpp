#include "datareceiver.h"

#include <messagetype.h>


DataReceiver::DataReceiver(QObject* parent):
    QObject(parent)
{

}

void DataReceiver::slotReceivedData() {
    QTcpSocket* senderSocket = (QTcpSocket*)QObject::sender();
    QByteArray data = senderSocket->readAll();

    if(data.contains("DEBUG_ENDPOINT_IDENT"))
        emit signalReceivedEndpointIdent(senderSocket, "debugEP", "testtype", "FF:FF");
    else
        processProtocollHeader(senderSocket, data);
}

int DataReceiver::processProtocollHeader(QTcpSocket* socket, QByteArray data) {
    QByteArray payload, splitOfPayload;
    QList<QByteArray>  messageParts;
    MessageType messageType;
    quint16 payloadLength=0;

    //check if StartOfHeader Code is at(0)
    if (data.at(0) != 0x01)
        return -1;
    messageType = (MessageType)data.at(1); //second Byte
    QByteArray lengthBytes= data.mid(2,2);

    payloadLength |= (quint8)(lengthBytes.at(0));
    payloadLength = payloadLength << 8;
    payloadLength |= (quint8)(lengthBytes.at(1));

    //get payload section (from 0x02 ..0x03)
    messageParts = data.split(0x02);
    if ( messageParts.length() >= 2 ) {
        splitOfPayload = messageParts.at(1);
    } else {
        return -2;
    }
    payload = splitOfPayload.split(0x03).at(0);
    //check payload length
    if (payload.length() != payloadLength) {
        return -3;
    }
    //check correct termination after payload section (0x03|0x04)
    QByteArray termination = data.mid(5 + payloadLength, 2);
    if (termination.length() >= 2 )
        if(termination.at(0) != 0x03 || termination.at(1) != 0x04 )
            return -4;

    processMessage(socket, messageType, payload);
    return 0;
}


void DataReceiver::processMessage(QTcpSocket* socket, MessageType type, QByteArray payload) {
    QString alias, MAC, endpointType, state, pass;
    bool switchedState;
    //0x1F = unit separator
    QList<QByteArray> payloadParts = payload.split(0x1F);

    switch(type) {
    case MESSAGETYPE_ENDPOINT_INFO:
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
   /*case MESSAGETYPE_ENDPOINTS_STATES_LIST:
        qDebug()<<"Number of Payload Parts: "<<payloadParts.length();

        for(int endpoint= 0; i<payloadParts.length(); i+=4) {
            alias   =   payloadParts.at(endpoint + 0);
            MAC     =   payloadParts.at(endpoint + 1);
            endpointType = payloadParts.at(endpoint + 2);
            state = payloadParts.at(endpoint + 3);
            newEndpoint = new Endpoint(NULL, alias, endpointType, MAC);
            if(state == "1") {
                newEndpoint->setState(true);
            }
            else {
                    newEndpoint->setState(false);
            }
            endpointsUpdate.append(newEndpoint);
        }
        emit signalReceivedEndpointList(endpointsUpdate);
        break;*/
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
    case MESSAGETYPE_UI_DELETE_ENTDPOINT:
        qDebug()<<__FUNCTION__<<"Received delete-endpoint message";
        emit signalDeleteEndpoint();
        break;
    case MESSAGETYPE_RESET_SERVER:
        emit signalResetServer();
        qDebug()<<__FUNCTION__<<"Received server-reset message";
        break;

    default:
        qDebug()<<__FUNCTION__<<"Unrecognized MessageType";
    }
}


