#include <chrono>

#include "sender.h"
#include "main.h"

Sender::Sender(DrawArea *drawArea, pthread_mutex_t *comms_mutex, QObject *parent)
    : QObject(parent), m_SendQueue(new SafeQueue<char>), CommsMutex(comms_mutex)
{
    /* connect all the signals from the draw area so that we can seriase our commands in real time */
    connect(drawArea, SIGNAL(lineDrawn(const QLine &)), this, SLOT(serializeLine(const QLine &)), Qt::QueuedConnection);
    connect(drawArea, SIGNAL(drawAreaCleared(const QColor &)), this, SLOT(serializeClear(const QColor &)), Qt::QueuedConnection);
    connect(drawArea, SIGNAL(penColourChanged(const QColor &)), this, SLOT(serializePenColour(const QColor &)), Qt::QueuedConnection);
    connect(drawArea, SIGNAL(penWidthChanged(const int)), this, SLOT(serializePenWidth(const int)), Qt::QueuedConnection);
}


Sender::~Sender()
{
    delete m_SendQueue;
}

/*
 * This function takes a byte from the queue and sends it as a stream of bits using the
 * global bools Comms::data and Comms::send_receive
 * The send_recieve flag is used to ensure we dont transmit before the reciever is ready
 * The send function can only take the flag high and the recieve function can only take it low
 */

void Sender::send()
{
    if(!m_SendQueue->isEmpty())
    {
        char send_byte =  m_SendQueue->front();

#ifdef DEBUG_BITRATE                                                            // if we want to know the bitrate for each message
        auto t1 = std::chrono::high_resolution_clock::now();                    // take a time before transmisson of a byte has started
#endif

        for(int i = 0; i < 8; i++)
        {
            pthread_mutex_lock(CommsMutex);
            Comms::data = (send_byte & (1 << i));                               // load the data bool from the 'i'th bit of send_byte
#ifdef DEBUG_COMMS
            qDebug() << "S(" << i << "): " << Comms::data;
#endif
            pthread_mutex_unlock(CommsMutex);

            pthread_mutex_lock(CommsMutex);
            Comms::sending = true;                                              // set the sending flag high to tell reciver its about to get a message
            pthread_mutex_unlock(CommsMutex);

            while(isReceived() == false);                                       // wait for the recived flag to go high, signifiying reception of the data

            pthread_mutex_lock(CommsMutex);
            Comms::sending = false;                                             // clear the sending flag
            pthread_mutex_unlock(CommsMutex);

            while(isReceived() == true);                                        // wait until send flag goes low, siginifying data has been read and is ready for next message
        }

#ifdef DEBUG_BITRATE                                                            // if we want to know the bitrate for each message
        static long long accumulator = 0, counter = 0;
        auto t2 = std::chrono::high_resolution_clock::now();                    // take a time after transmission of a byte has completed
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        accumulator += duration;                                                // compute the difference in time (how long did it take to send the message)
        counter ++;                                                             // add it to the accumulator and increment the counter
        if(counter == sizeof(m_SendMessage.vals))                               // when we have send an entire message
        {
            double result = 1/(((accumulator / counter) / 8) * 0.000001);       // find out how long the average byte took to send, divide by 8 to get time per bit and *10-6 for s
            qDebug() << "Average rate i: " << result << "bps";                  // print the result
            counter = 0;                                                        // reset the variables
            accumulator = 0;
        }
#endif

        m_SendQueue->pop();                                                     // once we have sent our byte, pop if off the queue so we can send the next one
    }
}

/*
 * When we are sending a 'line' message it has the format:
 * 'L'xxyyxxyy cc
 * where N is a colour value, x&y are coordinates and c is the checksum
 */

void Sender::serializeLine(const QLine &l)
{
    m_SendQueue->push('L');                                                     // we are sending a 'line' message

    m_SendMessage.data[0] = l.x1();                                             // fill the .data array with the coordinates of our line
    m_SendMessage.data[1] = l.y1();
    m_SendMessage.data[2] = l.x2();
    m_SendMessage.data[3] = l.y2();

    encodeMessage();                                                            // now that the data is loaded, we can encode our message
}

/*
 * When we are sending a 'clear' message, it has the format:
 * 'C''#NNNNNN' 0 cc
 * where N is a colour value, 0 padding to make the message 10 bytes long and c is the checksum
 */

void Sender::serializeClear(const QColor &c)
{
    m_SendQueue->push('C');                                                     // we are sending a 'clear' message
    sprintf(m_SendMessage.colour, c.name().toLatin1());                         // fill the .colour array with the colour we want to clear
    encodeMessage();                                                            // now that the data is loaded, we can encode our message
}

/*
 * When we are sending a 'Pen Colour' message, it has the format:
 * 'P''#NNNNNN' 0 cc
 * where N is a colour value, 0 padding to make the message 10 bytes long and c is the checksum
 */

void Sender::serializePenColour(const QColor &c)
{
    m_SendQueue->push('P');                                                     // we are sending a 'pen colour' message
    sprintf(m_SendMessage.colour, c.name().toLatin1());                         // fill the .colour array with the colour we want to clear
    encodeMessage();                                                            // now that the data is loaded, we can encode our message
}

/*
 * When we are sending a 'Pen Width' message, it has the format:
 * 'W' xx000000cc, where x is the chosen width, 0 padding and c is the checksumc
 */

void Sender::serializePenWidth(const int w)
{
    m_SendQueue->push('W');                                                     // we are sending a 'pen width' message
    m_SendMessage.data[0] = (int16_t)w;                                         // fill the first element of data with the width (we know max width is 30 so rollover not possible)
    encodeMessage();                                                            // now that the data is loaded, we can encode our message
}

void Sender::encodeMessage()                                                    // becasue all messages have the same format we can use this function fill the send queue up...
{
    for(int i = 0; i < (int)sizeof(m_SendMessage.data) / 2; i++)                // push the data (not including the checksum)
    {
        m_SendQueue->push(m_SendMessage.data[i] >> 8);                          // push the high byte
        m_SendQueue->push(m_SendMessage.data[i] & 0xFF);                        // push the low byte
    }

    m_SendMessage.checksum = checksum16(m_SendMessage.vals, sizeof(m_SendMessage.data));      // calculate the checksum

    m_SendQueue->push(m_SendMessage.checksum >> 8);                             // push the checksum high byte
    m_SendQueue->push(m_SendMessage.checksum & 0xFF);                           // push the checksum low byte
}


bool Sender::isReceived()
{
    bool ret;                                                                   // make a dummy variable
    pthread_mutex_lock(CommsMutex);
    ret = Comms::received;                                                      // set the dummy var
    pthread_mutex_unlock(CommsMutex);
    return ret;                                                                 // return the copy, enabling us to poll the send_recieve flag without keeping the mutex locked
}


