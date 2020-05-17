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
    explicit Receiver(ViewArea *viewArea = nullptr, pthread_mutex_t *comms_mutex = nullptr, QObject *parent = nullptr);

    void receive();

public slots:

signals:
    void lineReceived(const draw_data_t &dat);
    void clearReceived(const QColor &c);

private:
    QPixmap m_Pixmap;
    QByteArray m_ReceiveData;

    SafeQueue<char> *m_ReceiveQueue;

    pthread_mutex_t *CommsMutex;        // no m_ as this does not belong to the sender class, we just store a pointer to it

    void deserialse();
    bool isSending();
};

#endif // RECEIVER_H
