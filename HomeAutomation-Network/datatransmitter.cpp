#include "datatransmitter.h"

#define ALIAS   "Test UI 1"
#define MAC     "12:13:14:15:16:17"

using namespace std;

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

        sendMessage(MESSAGETYPE_UI_INFO, content);
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
        int success = this->client->write(message, message.length());
        if(success <= 0) {
            cout<<"Error writing to socket. Connected to "<<this->client->peerAddress().toString().toStdString()<<" "<<this->client->peerName().toStdString()<<"\n";
            return -1;
        }
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
    if( (payloadLength>>8) == 0) {
        message[2] = 0xFF;
    } else {
        message[2] =  (char)(payloadLength >> 8); //upper byte
    }
    message[3] =  (char)payloadLength;        //lower byte
    message[4] = 0x02;
    message.append(payload);
    message.append(0x03);
    message.append(0x04);
    //message.append(0x02); //payload END character
    //message.append(0x03); //message END character
    message.append(0x0D); //CR
    message.append(0x0A); //LF
    return message;
}



