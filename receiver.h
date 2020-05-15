#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QPixmap>

#include "typedefs.h"
#include "safequeue.h"
#include "receivewindow.h"

class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(ReceiveWindow *receiveWindow = nullptr, comms_signals_t *comms_signals = nullptr, QObject *parent = nullptr);

    void receive();

public slots:

signals:
    void pixmapReceived(const QPixmap &p);
    void dataReceived(const QByteArray &arr);

private:
    QPixmap m_Pixmap;
    QByteArray m_ReceiveData;

    SafeQueue<char> *m_ReceiveQueue;

    comms_signals_t *commsSignals;        // commsSignals point to the comms_signals instantiated in main, they do not belong to this class

    void deserialse();
    bool isSending();
    uint8_t asciiToHex(char c);
};

#endif // RECEIVER_H
