#include <QBuffer>
#include <QPixmap>

#include "sender.h"

Sender::Sender(DrawArea *drawArea, comms_signals_t *comms_signals, QObject *parent)
    : QObject(parent), m_OldDrawDataSize(0), m_SendQueue(new SafeQueue<char>), commsSignals(comms_signals)
{
    connect(drawArea, SIGNAL(pixmapUpdated(QPixmap &)), this, SLOT(serialisePixmap(QPixmap &)));
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

        for(int i=0; i<8; i++)
        {
            pthread_mutex_lock(&commsSignals->comms_mutex);
            commsSignals->data = (send_byte & (1 << i));
            pthread_mutex_unlock(&commsSignals->comms_mutex);

#ifdef DEBUG_COMMS
            qDebug() << "S(" << i << "): " << commsSignals->data;
#endif

            pthread_mutex_lock(&commsSignals->comms_mutex);
            commsSignals->sending = true;
            pthread_mutex_unlock(&commsSignals->comms_mutex);

            while(isReceiving() == false);         // wait until the recieve thread has got the data

            pthread_mutex_lock(&commsSignals->comms_mutex);
            commsSignals->sending = false;                        // stop sending, as the recieve thread has got the data
            pthread_mutex_unlock(&commsSignals->comms_mutex);

            while(isReceiving() == true);          // wait until recieve thread has finsihed recieving...

        }
        m_SendQueue->pop();                          // once we have sent our byte, pop if off the queue so we can send the next one
    }
}

void Sender::serialisePixmap(QPixmap &p)
{
    m_Pixmap = p;
    serialise();

    union16_t size = {.VAL = (uint16_t)m_SendData.size()};  // make a union16 vairable to hold the size
    m_SendQueue->push(size.BYTE0);                          // push the size in byte by byte, lsb first
    m_SendQueue->push(size.BYTE1);

    for(int i = 0; i < m_SendData.size(); i++)
    {
        m_SendQueue->push(m_SendData[i]);
    }
    qDebug() << "Send Size: " << m_SendData.size();
    m_OldDrawDataSize = m_SendData.size();                          // update the old draw data size so we only draw from here next time && needs to check if we have gone less...
}

void Sender::serialise()
{
    QBuffer buffer(&m_SendData);
    buffer.open(QIODevice::WriteOnly);
    m_Pixmap.save(&buffer, "PNG");
}

bool Sender::isReceiving()
{
    bool ret;
    pthread_mutex_lock(&commsSignals->comms_mutex);
    ret = commsSignals->receiving;
    pthread_mutex_unlock(&commsSignals->comms_mutex);
    return ret;
}

char Sender::hexToAscii(uint8_t d)
{
    if(d < 10)
        d += '0';
    else
        d += ('A' - 10);
    return d;
}
