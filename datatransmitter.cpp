#include "datatransmitter.h"

#define ALIAS   "Test UI 1"
#define MAC     "12:13:14:15:16:17"


DataTransmitter::DataTransmitter(QTcpSocket* client, QObject* parent):
    QObject(parent)
{
    if(client != 0)
        this->client = client;

}
void DataTransmitter::updateSocket(QTcpSocket *socket) {
    this->client = socket;
}

int DataTransmitter::sendInfoMessage() {
    if (this->client !=0) {
        QByteArray message;
        QByteArray content = "";
        content.append(ALIAS);
        content.append(0x1F);
        content.append(MAC);

        message = prepareMessage(MESSAGETYPE_UI_INFO, content);
        this->client->write(message, message.length());
        return 0;
    } else {
        return -1;
    }
}



int DataTransmitter::sendMessage(MessageType type, QByteArray payload) {
    if (this->client !=0) {
        QByteArray message;
        message = prepareMessage(type, payload);
        this->client->write(message, message.length());
         return 0;
    } else {
        return -1;
    }
}


QByteArray DataTransmitter::prepareMessage(MessageType type, QByteArray payload) {
    QByteArray message;
    quint16 payloadLength = (quint16)payload.length();

    message[0] = 0x01; //add start sign
    message[1] = (char)type;
    //message.append(payloadLength);

    message[2] =  (char)(payloadLength >> 8); //upper byte
    message[3] =  (char)payloadLength;        //lower byte
    message[4] = 0x02;
    message.append(payload);
    message.append(0x03);
    message.append(0x04);
    //message.at(n) = 0x02; //payload END character
    //message.at(n) = 0x03; //message END character

    //ToDo add everything
    return message;
}



