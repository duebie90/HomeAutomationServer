#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QtCore/QDebug>
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <webuiclient.h>
QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WsServer : public QObject
{
    Q_OBJECT
public:
    explicit WsServer(QString address, quint16 port, QObject *parent = 0);
    virtual ~WsServer();
signals:
public slots:
    void slotUpdateWebUis();
private slots:
    void onNewConnection();
    void clientDisconnected();
private:
    QWebSocketServer* webSocketServer;
    QList<WebUiClient *> clients;
};

#endif // WEBSOCKETSERVER_H
