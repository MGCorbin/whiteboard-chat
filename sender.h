#ifndef SENDER_H
#define SENDER_H

#include <QObject>

#include "typedefs.h"
#include "drawarea.h"
#include "safequeue.h"

class Sender : public QObject
{
    Q_OBJECT
public:
    explicit Sender(DrawArea *drawArea = nullptr, comms_signals_t *comms_signals = nullptr, QObject *parent = nullptr);
    ~Sender();

    void send();

public slots:
    void serialisePixmap(QPixmap &p);

signals:
    void serialDataUpdated(QByteArray &arr);

private:
    QPixmap m_Pixmap;
    QByteArray m_SendData;
    int m_OldDrawDataSize;

    SafeQueue<char> *m_SendQueue;

    comms_signals_t *commsSignals;        // comms signals point to the signals defined in main, they do not belong to this class

    void serialise();
    bool isReceiving();
    char hexToAscii(uint8_t d);

};

#endif // SENDER_H
