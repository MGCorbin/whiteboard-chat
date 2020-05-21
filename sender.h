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
    explicit Sender(DrawArea *drawArea = nullptr, pthread_mutex_t *comms_mutex = nullptr, QObject *parent = nullptr);
    ~Sender();

    void send();
    pthread_mutex_t *mutexPtr() const { return CommsMutex; }

public slots:
    void serializeLine(const QLine &l);
    void serializeClear(const QColor &c);
    void serializePenColour(const QColor &c);
    void serializePenWidth(const int w);

private:
    SafeQueue<char> *m_SendQueue;
    pthread_mutex_t *CommsMutex;        // no m_ as this does not belong to the sender class, we just store a pointer to it
    comms_message_t m_SendMessage;

    void serialiseColour();
    void encodeMessage();
    bool isReceived();
};


#endif // SENDER_H
