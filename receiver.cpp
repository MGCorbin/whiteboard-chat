#include "receiver.h"
#include "main.h"

Receiver::Receiver(ViewArea *viewArea, pthread_mutex_t *comms_mutex, QObject *parent)
    : QObject(parent), m_ReceiveQueue(new SafeQueue<char>), CommsMutex(comms_mutex)
{
    connect(this, SIGNAL(lineReceived(const QLine &)), viewArea, SLOT(drawLine(const QLine &)), Qt::QueuedConnection);
    connect(this, SIGNAL(clearReceived(const QColor &)), viewArea, SLOT(clearScreen(const QColor &)), Qt::QueuedConnection);
    connect(this, SIGNAL(penColourReceived(const QColor &)), viewArea, SLOT(setPenColour(const QColor &)), Qt::QueuedConnection);
    connect(this, SIGNAL(penWidthReceived(const int)), viewArea, SLOT(setPenWidth(const int)), Qt::QueuedConnection);
}

void Receiver::receive()
{
    char receive_byte = 0;

    for(int i=0; i<8; i++)
    {
        pthread_mutex_lock(CommsMutex);
        Comms::received = false;                            // initaially we have recieved nothing...
        pthread_mutex_unlock(CommsMutex);

        while(isSending() == false && Comms::enable);       // wait until we see the send flag go high, when it goes high, data is ready to be read
                                                            // enable must also be checked to stop us from waiting here and holding up the program if the user closes the windows
        pthread_mutex_lock(CommsMutex);
        receive_byte |= Comms::data << i;                   // load data into the 'i'th bit of recieve_byte
#ifdef DEBUG_COMMS
       qDebug() << "R(" << i << "): " << Comms::data;
#endif
        pthread_mutex_unlock(CommsMutex);

        pthread_mutex_lock(CommsMutex);
        Comms::received = true;                             // now that we have the data, can set the recived flag true, telling the sender we are ready for more data
        pthread_mutex_unlock(CommsMutex);

        while(isSending() == true);                         // wait until we see the send flag go low (a new message is being prepared)
    }

    m_ReceiveQueue->push(receive_byte);

    /*
     * at this point we know that the received flag is true
     * the sender will not give us any more data until we set received false
     * we can therefore process our data here, knowing we wont get any more until we have finished
     */

    if(m_ReceiveQueue->size() == sizeof(m_ReceiveMessage.vals) + 1)     // 10 byte message + 1 for the message ID
    {
        handleDeserialize();
    }
}

void Receiver::handleDeserialize()
{
    switch(m_ReceiveQueue->front())
    {
        case 'L':       deserializeLine();          break;
        case 'C':       deserializeClear();         break;
        case 'P':       deserializePenColour();     break;
        case 'W':       deserializePenWidth();      break;
        default:
        {
            qDebug() << "Unknown Command!";

            while(!m_ReceiveQueue->isEmpty())                   // if we get an unknown command, the best we can do is empty the queue, ready for the next message
            {
                m_ReceiveQueue->pop();
            }
            break;
        }
    }
}

void Receiver::deserializeLine()
{
    m_ReceiveQueue->pop();                                      // pop off the 'D', we dont need this to construct our line...
    decodeMessage();                                            // we have recieved a message, as all messages have the same format we can decode them in the same way...

    if(m_ReceiveMessage.checksum == checksum16(m_ReceiveMessage.vals, sizeof(m_ReceiveMessage.data)))
    {                                                           // if the received checksum == claculated checksum, emit the new line
        emit lineReceived(QLine(m_ReceiveMessage.data[0], m_ReceiveMessage.data[1], m_ReceiveMessage.data[2], m_ReceiveMessage.data[3]));
    }
    else
    {
        qDebug() << "Draw Message Checksum Error!";

        while(!m_ReceiveQueue->isEmpty())                       // if we have a checksum error, the best we can do is empty the queue, ready for next message
        {
            m_ReceiveQueue->pop();
        }
    }
}

void Receiver::deserializeClear()
{
    m_ReceiveQueue->pop();

    decodeMessage();

    if(m_ReceiveMessage.checksum == checksum16(m_ReceiveMessage.vals, sizeof(m_ReceiveMessage.data)))
    {
        emit clearReceived(QColor(m_ReceiveMessage.colour));
    }
    else
    {
        qDebug() << "Clear Message Checksum Error!";

        while(!m_ReceiveQueue->isEmpty())                       // if we have a checksum error, the best we can do is empty the queue, ready for next message
        {
            m_ReceiveQueue->pop();
        }
    }
}

void Receiver::deserializePenColour()
{
    m_ReceiveQueue->pop();
    decodeMessage();

    if(m_ReceiveMessage.checksum == checksum16(m_ReceiveMessage.vals, sizeof(m_ReceiveMessage.data)))
    {
        emit penColourReceived(QColor(m_ReceiveMessage.colour));
    }
    else
    {
        qDebug() << "Pen Colour Message Checksum Error!";

        while(!m_ReceiveQueue->isEmpty())                       // if we have a checksum error, the best we can do is empty the queue, ready for next message
        {
            m_ReceiveQueue->pop();
        }
    }
}

void Receiver::deserializePenWidth()
{
    m_ReceiveQueue->pop();
    decodeMessage();

    if(m_ReceiveMessage.checksum == checksum16(m_ReceiveMessage.vals, sizeof(m_ReceiveMessage.data)))
    {
        emit penWidthReceived((int)m_ReceiveMessage.data[0]);
    }
    else
    {
        qDebug() << "Pen Width Message Checksum Error!";

        while(!m_ReceiveQueue->isEmpty())                           // if we have a checksum error, the best we can do is empty the queue, ready for next message
        {
            m_ReceiveQueue->pop();
        }
    }
}

void Receiver::decodeMessage()                                          // this function fills the m_RecieveData variable with stuff from the queue...
{                                                                       // as all messages have same format we can use this general function for every message
    uint8_t high, low;
    for(int i = 0; i < (int)sizeof(m_ReceiveMessage.data) / 2; i++)     // get the data from the queue
    {
        high = m_ReceiveQueue->front();                                  // load up the temporary high variable
        m_ReceiveQueue->pop();
        low= m_ReceiveQueue->front();                                    // load up the temporary low variable
        m_ReceiveQueue->pop();
        m_ReceiveMessage.data[i] = low | (high << 8);                   // set the data (each data element is a uint16_t)
    }

    high = m_ReceiveQueue->front();                                      // repeat this to get the checksum
    m_ReceiveQueue->pop();
    low = m_ReceiveQueue->front();
    m_ReceiveQueue->pop();
    m_ReceiveMessage.checksum = low | (high << 8);                      // set the .checksum of our receiveMessage
}

bool Receiver::isSending()                                          // this function allows us to poll the flag without keeping the mutex locked
{
    bool ret;                                                           // setup a dummy variable
    pthread_mutex_lock(CommsMutex);
    ret = Comms::sending;                                               // read the value into the dummy var while the mutex is locked
    pthread_mutex_unlock(CommsMutex);
    return ret;                                                         // return the dummy after unlcoking
}

