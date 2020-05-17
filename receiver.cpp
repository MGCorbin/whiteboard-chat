#include "receiver.h"

Receiver::Receiver(ViewArea *viewArea, pthread_mutex_t *comms_mutex, QObject *parent)
    : QObject(parent), m_Pixmap(800, 600), m_ReceiveQueue(new SafeQueue<char>), CommsMutex(comms_mutex)
{
    connect(this, SIGNAL(lineReceived(const draw_data_t &)), viewArea, SLOT(drawLine(const draw_data_t &)), Qt::QueuedConnection);
    connect(this, SIGNAL(clearReceived(const QColor &)), viewArea, SLOT(clearScreen(const QColor &)));
}

void Receiver::receive()
{
    char receive_byte = 0;
    for(int i=0; i<8; i++)
    {

        while(isSending() == false);                        // wait until we see the send flag go high...

        pthread_mutex_lock(CommsMutex);
        receive_byte |= Comms::data << i;            // load in the data to the correct position on the receive byte
        pthread_mutex_unlock(CommsMutex);

#ifdef DEBUG_COMMS
        qDebug() << "R<" << i << ">: " << commsSignals->data;
#endif

        pthread_mutex_lock(CommsMutex);
        Comms::send_receive = false;
        pthread_mutex_unlock(CommsMutex);
    }

    /*
     * at this point we know that receiving signal is true, therefore if we process data here,
     * we can be sure that we will not recieve any more data until we restart the for loop
     * (after we have finished processing)
     */

    m_ReceiveQueue->push(receive_byte);

    static int counter = 0;
    counter ++;

    if(counter == 17)
    {
        counter = 0;
        deserialse();
    }

//    qDebug() << "Received byte: " << receive_byte;
//    qDebug() << " ";
}

void Receiver::deserialse()
{
    QByteArray raw_colour;

    if(m_ReceiveQueue->font() == '<')
    {
        m_ReceiveQueue->pop();

        for(int i = 0; i < 7; i++)
        {
            raw_colour.push_back(m_ReceiveQueue->font());
            m_ReceiveQueue->pop();
        }

        union16_t x1;
        union16_t y1;
        union16_t x2;
        union16_t y2;

        x1.BYTE1 = m_ReceiveQueue->font();
        m_ReceiveQueue->pop();
        x1.BYTE0 = m_ReceiveQueue->font();
        m_ReceiveQueue->pop();
        y1.BYTE1 = m_ReceiveQueue->font();
        m_ReceiveQueue->pop();
        y1.BYTE0 = m_ReceiveQueue->font();
        m_ReceiveQueue->pop();
        x2.BYTE1 = m_ReceiveQueue->font();
        m_ReceiveQueue->pop();
        x2.BYTE0 = m_ReceiveQueue->font();
        m_ReceiveQueue->pop();
        y2.BYTE1 = m_ReceiveQueue->font();
        m_ReceiveQueue->pop();
        y2.BYTE0 = m_ReceiveQueue->font();
        m_ReceiveQueue->pop();

        if(m_ReceiveQueue->font() == '>')
        {
            m_ReceiveQueue->pop();
            draw_data_t new_data;
            new_data.col.setNamedColor(raw_colour);
            new_data.line = QLine(x1.VAL, y1.VAL, x2.VAL, y2.VAL);

            if(new_data.line == QLine(0x7F7F, 0x7F7F, 0x7F7F, 0x7F7F))
            {
                emit clearReceived(new_data.col);
            }
            else
            {
                emit lineReceived(new_data);
            }
        }

//        qDebug() << "Recieved colour: " << raw_colour;

    }
    else
    {
        qDebug() << "ERROR!";
        /* reception error! */
    }
}

bool Receiver::isSending()
{
    bool ret;
    pthread_mutex_lock(CommsMutex);
    ret = Comms::send_receive;
    pthread_mutex_unlock(CommsMutex);
    return ret;
}

