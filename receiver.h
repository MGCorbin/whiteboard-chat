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
    pthread_mutex_t *mutexPtr() const { return CommsMutex; }

signals:
    void lineReceived(const QLine &l);
    void clearReceived(const QColor &c);
    void penColourReceived(const QColor &c);
    void penWidthReceived(const int w);

private:
    SafeQueue<char> *m_ReceiveQueue;
    pthread_mutex_t *CommsMutex;        // no m_ as these do not belong to the sender class, we just store a pointer to them
    pthread_mutex_t *StopMutex;
    comms_message_t m_ReceiveMessage;

    void handleDeserialize();
    void deserializeLine();
    void deserializeClear();
    void deserializePenColour();
    void deserializePenWidth();
    void decodeMessage();
    bool isSending();
};

#endif // RECEIVER_H
