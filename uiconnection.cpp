#include "uiconnection.h"
#include <QtNetwork>


UiConnection::UiConnection(QTcpSocket* socket, QString alias)
{
    cout<<"UI Connection Object created with alias "<<alias.toStdString()<<".\n";
    this->clientSocket = socket;
    this->alias = alias;
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slotReceivedData()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));

}

void UiConnection::slotReceivedData() {
    QByteArray data = this->clientSocket->readAll();
    QString message = QString(data);
    QHostAddress remoteAddress = clientSocket->peerAddress();
    cout<<__FUNCTION__;
    cout<<"Alias: "<<alias.toStdString()<<" IP:"<<remoteAddress.toString().toStdString()<<" data:"<<message.toStdString()<<"\n";
}

void UiConnection::slotDisconnected() {
    cout<<"Client Disconnected\n";
}

