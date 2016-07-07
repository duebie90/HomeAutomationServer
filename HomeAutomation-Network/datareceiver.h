#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include <QObject>
#include <messagetype.h>
#include <QtNetwork>
#include <ScheduleEvent.h>

/* Diese Klasse stellt einen Protokoll Parser dar.
 * Sie erkennt den Typ der Nachricht und überprüft die Korrekte Form aufgrund des
 * Telegramm-Headers.
 * Anschließend interpretiert sie den Payload/ Inhalt der Nachricht je nach Typ
 * und ruft ein Signal für die weiterverarbeitung auf
 */
class DataReceiver: public QObject
{
    Q_OBJECT
public:
    DataReceiver(QObject* parent=0);

public slots:
    void slotReceivedData();
signals:
    //void signalReceivedEndpointList(Endpoint* endpoint);//QList<Endpoint*> endpointsUpdate);
    void signalReceivedEndpointIdent(QTcpSocket* socket, QString alias, QString type, QString MAC);
    void signalReceivedEndpointState(QString MAC, bool state);
    void signalReceivedUiEndpointStateRequest(QString MAC, bool state);
    void signalReceivedServerEndpointStateRequest(QString MAC, bool state);
    void signalReceivedUiIdent(QTcpSocket* socket, QString alias, QString pass, QString MAC);
    void signalReceivedEndpointSchedule(QString mac, ScheduleEvent* scheduleEvent);
    void signalDeleteEndpoint();
    void signalResetServer();
    //...
private:
    int processProtocollHeader(QTcpSocket* socket, QByteArray data);
    void processMessage(QTcpSocket* socket, MessageType type, QByteArray message);

};

#endif // DATARECEIVER_H
