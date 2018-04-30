#ifndef WEBUICLIENT_H
#define WEBUICLIENT_H

#include <QObject>
#include "QtWebSockets/QWebSocket"
#include <endpoint.h>
class WebUiClient : public QObject
{
    Q_OBJECT
public:
    explicit WebUiClient(QWebSocket* socket, QObject *parent = 0);
    void updateWebUi(QList<AbstractEndpoint*> endpoints);
signals:
    void disconnected();
private slots:
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
private:
    bool sendBinaryMessage(QByteArray message);
    QWebSocket * webSocket;

};

#endif // WEBUICLIENT_H
