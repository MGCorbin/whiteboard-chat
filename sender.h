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

public slots:
    void serialiseData(const draw_data_t &dat);
    void serialiseData(const QColor &c);

signals:
    void serialDataUpdated(QByteArray &arr);

private:
    QPixmap m_Pixmap;
    QByteArray m_SendData;
    int m_OldDrawDataSize;

    SafeQueue<char> *m_SendQueue;

    pthread_mutex_t *CommsMutex;        // no m_ as this does not belong to the sender class, we just store a pointer to it

    void serialiseColour();

    bool isReceiving();
    bool isSending();
};

#endif // SENDER_H
