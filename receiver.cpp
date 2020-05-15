#include "receiver.h"

Receiver::Receiver(ReceiveWindow *receiveWindow, comms_signals_t *comms_signals, QObject *parent)
    : QObject(parent), m_Pixmap(800, 600), m_ReceiveQueue(new SafeQueue<char>), commsSignals(comms_signals)
{
    connect(this, SIGNAL(pixmapReceived(const QPixmap&)), receiveWindow, SLOT(updatePixmap(const QPixmap&)));
//    connect(this, SIGNAL(dataReceived(const QByteArray&)), receiveWindow, SLOT(updateArray(const QByteArray&)));
}

void Receiver::receive()
{
    char receive_byte = 0;
    for(int i=0; i<8; i++)
    {
        pthread_mutex_lock(&commsSignals->comms_mutex);
        commsSignals->receiving = false;                    // initially we are not receiving
        pthread_mutex_unlock(&commsSignals->comms_mutex);

        while(isSending() == false);                        // wait until we see the send flag go high...

        pthread_mutex_lock(&commsSignals->comms_mutex);
        receive_byte |= commsSignals->data << i;            // load in the data to the correct position on the receive byte
        pthread_mutex_unlock(&commsSignals->comms_mutex);

#ifdef DEBUG_COMMS
        qDebug() << "R<" << i << ">: " << commsSignals->data;
#endif

        pthread_mutex_lock(&commsSignals->comms_mutex);
        commsSignals->receiving = true;                     // once we have got the data we can tell the send thread that we have received
        pthread_mutex_unlock(&commsSignals->comms_mutex);

        while(isSending() == true);                         // wait until we see the send flag go low
    }

    /*
     * at this point we know that receiving signal is true, therefore if we process data here,
     * we can be sure that we will not recieve any more data until we restart the for loop
     * (after we have finished processing)
     */

//    qDebug() << "Received byte: " << receive_byte;
//    qDebug() << " ";

    static bool got_size = false;
    union16_t size;

    m_ReceiveQueue->push(receive_byte);      // once we have the byte, push it into the receive queue

    if(m_ReceiveQueue->size() == 2 && !got_size)
    {
        size.BYTE0 = m_ReceiveQueue->font();
        m_ReceiveQueue->pop();
        size.BYTE1 = m_ReceiveQueue->font();
        m_ReceiveQueue->pop();
        got_size = true;
        qDebug() << "Recieve Size: " << size.VAL;
    }
    else if(m_ReceiveQueue->size() == size.VAL)
    {
        deserialse();
        got_size = false;
    }
}

void Receiver::deserialse()
{
    while(!m_ReceiveQueue->isEmpty())
    {
        m_ReceiveData.push_back(m_ReceiveQueue->font());
        //qDebug() << "Elem(" << i++ << "): " << m_ReceiveQueue->font();
        m_ReceiveQueue->pop();
    }

    m_Pixmap.loadFromData(m_ReceiveData, "PNG");
    emit pixmapReceived(m_Pixmap);
    m_ReceiveData.clear();

//    emit dataReceived(m_ReceiveData);
}

bool Receiver::isSending()
{
    bool ret;
    pthread_mutex_lock(&commsSignals->comms_mutex);
    ret = commsSignals->sending;
    pthread_mutex_unlock(&commsSignals->comms_mutex);
    return ret;
}

uint8_t Receiver::asciiToHex(char c)
{
    if(c > '9')
        c -= ('A' - 10);
    else
        c -= '0';
    return c;
}
