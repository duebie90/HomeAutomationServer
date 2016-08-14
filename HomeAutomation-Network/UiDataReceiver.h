#ifndef UiDataReceiver_H
#define UiDataReceiver_H

#include <QObject>
#include <messagetype.h>
#include <QtNetwork>
#include <../HomeAutomation-Devices/ScheduleEvent.h>

class ScheduleEvent;

/* Diese Klasse stellt einen Protokoll Parser dar.
 * Sie erkennt den Typ der Nachricht und überprüft die Korrekte Form aufgrund des
 * Telegramm-Headers.
 * Anschließend interpretiert sie den Payload/ Inhalt der Nachricht je nach Typ
 * und ruft ein Signal für die weiterverarbeitung auf
 */
class UiDataReceiver: public QObject
{
    Q_OBJECT
public:
    UiDataReceiver(QObject* parent=0);

public slots:
    void slotReceivedData();
signals:
    void signalReceivedEndpointState(QString MAC, bool state);
    void signalReceivedUiEndpointStateRequest(QString MAC, bool state);
    void signalReceivedServerEndpointStateRequest(QString MAC, bool state);
    void signalReceivedUiIdent(QTcpSocket* socket, QString alias, QString pass, QString MAC);
    void signalReceivedEndpointSchedule(QString mac, ScheduleEvent* scheduleEvent);
    void signalReceivedAutoRequest(QString mac, bool autoControlled);
    void signalDeleteEndpoint(QString mac);
    void signalResetServer();
    void signalDeleteSchedule(QString mac, int id);
    //...
private:
    int processProtocollHeader(QTcpSocket* socket, QByteArray data);
    void processMessage(QTcpSocket* socket, MessageType type, QByteArray message);

};

#endif // UiDataReceiver_H
