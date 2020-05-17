#include <QBuffer>
#include <QPixmap>
#include <chrono>

#include "sender.h"

Sender::Sender(DrawArea *drawArea, pthread_mutex_t *comms_mutex, QObject *parent)
    : QObject(parent), m_OldDrawDataSize(0), m_SendQueue(new SafeQueue<char>), CommsMutex(comms_mutex)
{
    connect(drawArea, SIGNAL(lineDrawn(const draw_data_t &)), this, SLOT(serialiseData(const draw_data_t &)), Qt::QueuedConnection);
    connect(drawArea, SIGNAL(screenCleared(const QColor &)), this, SLOT(serialiseData(const QColor &)));
}


Sender::~Sender()
{

}

void Sender::send()
{
    if(!m_SendQueue->isEmpty())
    {
        char send_byte =  m_SendQueue->font();
//        qDebug() << "Queue size:" << m_SendQueue->size();
//        qDebug() << "Send byte: " << send_byte;

        auto t1 = std::chrono::high_resolution_clock::now();

        for(int i=0; i<8; i++)
        {
            pthread_mutex_lock(CommsMutex);
            Comms::data = (send_byte & (1 << i));
            pthread_mutex_unlock(CommsMutex);

#ifdef DEBUG_COMMS
            qDebug() << "S(" << i << "): " << commsSignals->data;
#endif

            pthread_mutex_lock(CommsMutex);
            Comms::send_receive = true;
            pthread_mutex_unlock(CommsMutex);

            while(isSending() == true);         // wait until the recieve thread has got the data
        }

        auto t2 = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

//        qDebug() << "Duration: " << duration;

        m_SendQueue->pop();                          // once we have sent our byte, pop if off the queue so we can send the next one
    }
}

void Sender::serialiseData(const draw_data_t &dat)
{
    QByteArray raw_colour = dat.col.name().toLatin1();

    m_SendQueue->push('<');

    for(int i = 0; i < raw_colour.size(); i++)
    {
        m_SendQueue->push(raw_colour.at(i));
    }

    union16_t x1 = {.VAL = (int16_t)dat.line.x1()};
    union16_t y1 = {.VAL = (int16_t)dat.line.y1()};
    union16_t x2 = {.VAL = (int16_t)dat.line.x2()};
    union16_t y2 = {.VAL = (int16_t)dat.line.y2()};

    m_SendQueue->push(x1.BYTE1);
    m_SendQueue->push(x1.BYTE0);
    m_SendQueue->push(y1.BYTE1);
    m_SendQueue->push(y1.BYTE0);
    m_SendQueue->push(x2.BYTE1);
    m_SendQueue->push(x2.BYTE0);
    m_SendQueue->push(y2.BYTE1);
    m_SendQueue->push(y2.BYTE0);

    m_SendQueue->push('>');
}

void Sender::serialiseData(const QColor &c)
{
    QByteArray raw_colour = c.name().toLatin1();

    m_SendQueue->push('<');

    for(int i = 0; i < raw_colour.size(); i++)
    {
        m_SendQueue->push(raw_colour.at(i));
    }

    for(int i = 0; i < (PACKET_SIZE - raw_colour.size()) - 2; i++)
    {
        m_SendQueue->push(0x7F);
    }

    m_SendQueue->push('>');
    qDebug() << "Send Queue Size: " << m_SendQueue->size();
}


bool Sender::isSending()
{
    bool ret;
    pthread_mutex_lock(CommsMutex);
    ret = Comms::send_receive;
    pthread_mutex_unlock(CommsMutex);
    return ret;
}
